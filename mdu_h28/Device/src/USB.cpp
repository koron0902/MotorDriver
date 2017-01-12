/*
 * USB.cpp
 *
 *  Created on: 2016/11/14
 *      Author: takumi152
 */

#include <chip.hpp>
#include <ring_buffer.hpp>
#include <string.h>
#include <USB.hpp>
#include "usb/app_usbd_cfg.h"
#include <algorithm>
#include <iap.h>
#include <port.hpp>

using namespace std;

namespace Device {

namespace USB {

static USBD_HANDLE_T g_hUsb;
const USBD_API_T *g_pUsbApi;
//static uint8_t g_rxBuff[256]; // For testing only
struct VCOM_DATA {
	USBD_HANDLE_T hUsb;
	USBD_HANDLE_T hCdc;
	uint8_t *rx_buff;
	uint16_t rx_rd_count;
	uint16_t rx_count;
	volatile uint16_t tx_flags;
	volatile uint16_t rx_flags;
};
static VCOM_DATA g_vCOM;

/**
 * @brief	Virtual com port init routine
 * @param	hUsb		: Handle to USBD stack instance
 * @param	pDesc		: Pointer to configuration descriptor
 * @param	pUsbParam	: Pointer USB param structure returned by previous init call
 * @return	Always returns LPC_OK.
 */

static USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc,
		uint32_t intfClass);
static ErrorCode_t vcom_init(USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc,
		USBD_API_INIT_PARAM_T *pUsbParam);

//キューとして実装
static uint8_t RxRaw[RxBufferSize];
static RINGBUFF_T RxBuf;
//単純配列として実装。
static char TxBuf[TxBufferSize];
static size_t TxPos;

/* Find the address of interface descriptor for given class type. */
USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc,
		uint32_t intfClass) {
	USB_COMMON_DESCRIPTOR *pD;
	USB_INTERFACE_DESCRIPTOR *pIntfDesc = 0;
	uint32_t next_desc_adr;

	pD = (USB_COMMON_DESCRIPTOR *) pDesc;
	next_desc_adr = (uint32_t) pDesc;

	while (pD->bLength) {
		/* is it interface descriptor */
		if (pD->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE) {

			pIntfDesc = (USB_INTERFACE_DESCRIPTOR *) pD;
			/* did we find the right interface descriptor */
			if (pIntfDesc->bInterfaceClass == intfClass) {
				break;
			}
		}
		pIntfDesc = 0;
		next_desc_adr = (uint32_t) pD + pD->bLength;
		pD = (USB_COMMON_DESCRIPTOR *) next_desc_adr;
	}

	return pIntfDesc;
}

void Init() {
	USBD_API_INIT_PARAM_T usb_param;
	USB_CORE_DESCS_T desc;
	ErrorCode_t ret = LPC_OK;
	uint32_t prompt = 0, rdCnt = 0;  // For testing only

	//Clock Supply
	Chip_USB_Init();

	// initialize USBD ROM API pointer
	g_pUsbApi = (const USBD_API_T *) LPC_ROM_API->pUSBD;

	// initialize call back structures
	memset((void *) &usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
	usb_param.usb_reg_base = LPC_USB0_BASE;
	usb_param.max_num_ep = 5 + 1;
	usb_param.mem_base = USB_STACK_MEM_BASE;
	usb_param.mem_size = USB_STACK_MEM_SIZE;

	// Set the USB descriptors
	desc.device_desc = (uint8_t *) &USB_DeviceDescriptor[0];
	desc.string_desc = (uint8_t *) &USB_StringDescriptor[0];

	/* Note, to pass USBCV test full-speed only devices should have both
	 * descriptor arrays point to same location and device_qualifier set
	 * to 0.
	 */
	desc.high_speed_desc = (uint8_t *) &USB_FsConfigDescriptor[0];
	desc.full_speed_desc = (uint8_t *) &USB_FsConfigDescriptor[0];
	desc.device_qualifier = 0;

	// USB Initialization
	ret = USBD_API->hw->Init(&g_hUsb, &desc, &usb_param);
	if (ret == LPC_OK) {
		// Init VCOM interface
		ret = vcom_init(g_hUsb, &desc, &usb_param);
		if (ret == LPC_OK) {
			msc_init(g_hUsb, &desc, &usb_param);
			//  enable USB interrupts
			NVIC_EnableIRQ(USB0_IRQn);
			// now connect
			USBD_API->hw->Connect(g_hUsb, 1);
		}
	}

	//Second Buffer
	RingBuffer_Init(&RxBuf,RxRaw,sizeof(RxRaw[0]),RxBufferSize);

}

/* VCOM bulk EP_IN endpoint handler */
static ErrorCode_t VCOM_bulk_in_hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	VCOM_DATA *pVcom = (VCOM_DATA *) data;

	if (event == USB_EVT_IN) {
		pVcom->tx_flags &= ~VCOM_TX_BUSY;
	}
	return LPC_OK;
}

/* VCOM bulk EP_OUT endpoint handler */
static ErrorCode_t VCOM_bulk_out_hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	VCOM_DATA *pVcom = (VCOM_DATA *) data;

	switch (event) {
	case USB_EVT_OUT:
		pVcom->rx_count = USBD_API->hw->ReadEP(hUsb, USB_CDC_OUT_EP, pVcom->rx_buff);
		if (pVcom->rx_flags & VCOM_RX_BUF_QUEUED) {
			pVcom->rx_flags &= ~VCOM_RX_BUF_QUEUED;
			if (pVcom->rx_count != 0) {
				pVcom->rx_flags |= VCOM_RX_BUF_FULL;
			}

		} else if (pVcom->rx_flags & VCOM_RX_DB_QUEUED) {
			pVcom->rx_flags &= ~VCOM_RX_DB_QUEUED;
			pVcom->rx_flags |= VCOM_RX_DONE;
		}
		break;

	case USB_EVT_OUT_NAK:
		/* queue free buffer for RX */
		if ((pVcom->rx_flags & (VCOM_RX_BUF_FULL | VCOM_RX_BUF_QUEUED)) == 0) {
			USBD_API->hw->ReadReqEP(hUsb, USB_CDC_OUT_EP, pVcom->rx_buff,
			VCOM_RX_BUF_SZ);
			pVcom->rx_flags |= VCOM_RX_BUF_QUEUED;
		}
		break;

	default:
		break;
	}

	return LPC_OK;
}

/* Set line coding call back routine */
static ErrorCode_t VCOM_SetLineCode(USBD_HANDLE_T hCDC,
		CDC_LINE_CODING *line_coding) {
	VCOM_DATA *pVcom = &g_vCOM;

	/* Called when baud rate is changed/set. Using it to know host connection state */
	pVcom->tx_flags = VCOM_TX_CONNECTED; /* reset other flags */

	return LPC_OK;
}

/* Virtual com port init routine */
ErrorCode_t vcom_init(USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc,
		USBD_API_INIT_PARAM_T *pUsbParam) {
	USBD_CDC_INIT_PARAM_T cdc_param;
	ErrorCode_t ret = LPC_OK;
	uint32_t ep_indx;

	g_vCOM.hUsb = hUsb;
	memset((void *) &cdc_param, 0, sizeof(USBD_CDC_INIT_PARAM_T));
	cdc_param.mem_base = pUsbParam->mem_base;
	cdc_param.mem_size = pUsbParam->mem_size;
	cdc_param.cif_intf_desc = (uint8_t *) find_IntfDesc(pDesc->high_speed_desc,
	CDC_COMMUNICATION_INTERFACE_CLASS);
	cdc_param.dif_intf_desc = (uint8_t *) find_IntfDesc(pDesc->high_speed_desc,
	CDC_DATA_INTERFACE_CLASS);
	cdc_param.SetLineCode = VCOM_SetLineCode;

	ret = USBD_API->cdc->init(hUsb, &cdc_param, &g_vCOM.hCdc);

	if (ret == LPC_OK) {
		/* allocate transfer buffers */
		g_vCOM.rx_buff = (uint8_t *) cdc_param.mem_base;
		cdc_param.mem_base += VCOM_RX_BUF_SZ;
		cdc_param.mem_size -= VCOM_RX_BUF_SZ;

		/* register endpoint interrupt handler */
		ep_indx = (((USB_CDC_IN_EP & 0x0F) << 1) + 1);
		ret = USBD_API->core->RegisterEpHandler(hUsb, ep_indx,
				VCOM_bulk_in_hdlr, &g_vCOM);
		if (ret == LPC_OK) {
			/* register endpoint interrupt handler */
			ep_indx = ((USB_CDC_OUT_EP & 0x0F) << 1);
			ret = USBD_API->core->RegisterEpHandler(hUsb, ep_indx,
					VCOM_bulk_out_hdlr, &g_vCOM);
		}
		/* update mem_base and size variables for cascading calls. */
		pUsbParam->mem_base = cdc_param.mem_base;
		pUsbParam->mem_size = cdc_param.mem_size;
	}

	return ret;
}

/* Virtual com port buffered read routine */
uint32_t vcom_bread(uint8_t *pBuf, uint32_t buf_len) {
	VCOM_DATA *pVcom = &g_vCOM;
	uint16_t cnt = 0;
	/* read from the default buffer if any data present */
	if (pVcom->rx_count) {
		cnt = (pVcom->rx_count < buf_len) ? pVcom->rx_count : buf_len;
		memcpy(pBuf, pVcom->rx_buff, cnt);
		pVcom->rx_rd_count += cnt;

		/* enter critical section */
		NVIC_DisableIRQ(USB0_IRQn);
		if (pVcom->rx_rd_count >= pVcom->rx_count) {
			pVcom->rx_flags &= ~VCOM_RX_BUF_FULL;
			pVcom->rx_rd_count = pVcom->rx_count = 0;
		}
		/* exit critical section */
		NVIC_EnableIRQ(USB0_IRQn);
	}
	return cnt;

}

/* Virtual com port read routine */
ErrorCode_t vcom_read_req(uint8_t *pBuf, uint32_t len) {
	VCOM_DATA *pVcom = &g_vCOM;

	/* check if we queued Rx buffer */
	if (pVcom->rx_flags & (VCOM_RX_BUF_QUEUED | VCOM_RX_DB_QUEUED)) {
		return ERR_BUSY;
	}
	/* enter critical section */
	NVIC_DisableIRQ(USB0_IRQn);
	/* if not queue the request and return 0 bytes */
	USBD_API->hw->ReadReqEP(pVcom->hUsb, USB_CDC_OUT_EP, pBuf, len);
	/* exit critical section */
	NVIC_EnableIRQ(USB0_IRQn);
	pVcom->rx_flags |= VCOM_RX_DB_QUEUED;

	return LPC_OK;
}

/* Gets current read count. */
uint32_t vcom_read_cnt(void) {
	VCOM_DATA *pVcom = &g_vCOM;
	uint32_t ret = 0;

	if (pVcom->rx_flags & VCOM_RX_DONE) {
		ret = pVcom->rx_count;
		pVcom->rx_count = 0;
	}

	return ret;
}

/* Virtual com port write routine*/
//uint32_t vcom_write(uint8_t *pBuf, uint32_t len) {
uint32_t WriteDirect(uint8_t *pBuf, uint32_t len) {
	VCOM_DATA *pVcom = &g_vCOM;
	uint32_t ret = 0;

	if ((pVcom->tx_flags & VCOM_TX_CONNECTED)
			&& ((pVcom->tx_flags & VCOM_TX_BUSY) == 0)) {
		pVcom->tx_flags |= VCOM_TX_BUSY;

		/* enter critical section */
		NVIC_DisableIRQ(USB0_IRQn);
		ret = USBD_API->hw->WriteEP(pVcom->hUsb, USB_CDC_IN_EP, pBuf, len);
		/* exit critical section */
		NVIC_EnableIRQ(USB0_IRQn);
	}

	return ret;
}

/* Mass storage device init routine */
ErrorCode_t msc_init(USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc, USBD_API_INIT_PARAM_T *pUsbParam){
	USBD_MSC_INIT_PARAM msc_param;
	ErrorCode_t ret = LPC_OK;

	/* init MSC params */
	msc_param.mem_base = pUsbParam->mem_base;
	msc_param.mem_size = pUsbParam->mem_size;
	msc_param.intf_desc = (uint8_t*) find_IntfDesc(pDesc->high_speed_desc, USB_DEVICE_CLASS_STORAGE);
	msc_param.InquiryStr = MSC_SCSI_InquiryString;
	msc_param.BlockCount = 32;
	msc_param.BlockSize = 512;
	msc_param.MemorySize = 16384;
	/* user defined functions */
	msc_param.MSC_Write = msc_write;
	msc_param.MSC_Read = msc_read;
	msc_param.MSC_Verify = msc_verify;

	ret = USBD_API->msc->init(hUsb, &msc_param);

	return ret;
}

void msc_write(uint32_t offset, uint8_t** src, uint32_t length, uint32_t high_offset){
	//TODO: msc write stub
	/* enter critical section */
	NVIC_DisableIRQ(USB0_IRQn);
	/*
	if(length<256){ //small length (64, 128)
		uint8_t* tmp;
		uint8_t buf[256];
		tmp = (uint8_t*)((uint32_t)0x0003C000+(offset&0xFFFFFF00));
		memcpy(buf, tmp, 256); //copy page data
		memcpy(&buf[offset%256], *src, length*sizeof(uint8_t)); //insert source data
		//Chip_EEPROM_Write(offset, *src, length*sizeof(uint8_t));
		Chip_IAP_PreSectorForReadWrite(0x3C+(offset&0xFFFFFF00)/0x1000, 0x3C+(offset+length-1)/0x1000);
		Chip_IAP_ErasePage(0x3C0+(offset&0xFFFFFF00)/0x100, 0x3C0+((offset+length-1)&0xFFFFFF00)/0x100);
		Chip_IAP_PreSectorForReadWrite(0x3C+(offset&0xFFFFFF00)/0x1000, 0x3C+(offset+length-1)/0x1000);
		Chip_IAP_CopyRamToFlash(0x0003C000+(offset&0xFFFFFF00),(uint32_t*)buf,256);
	}else{ //large length(256, 512, 1024)
		//Chip_EEPROM_Write(offset, *src, length*sizeof(uint8_t));
		Chip_IAP_PreSectorForReadWrite(0x3C+offset/0x1000, 0x3C+(offset+length-1)/0x1000);
		Chip_IAP_ErasePage(0x3C0+(offset&0xFFFFFF00)/0x100, 0x3C0+((offset+length-1)&0xFFFFFF00)/0x100);
		Chip_IAP_PreSectorForReadWrite(0x3C+offset/0x1000, 0x3C+(offset+length-1)/0x1000);
		Chip_IAP_CopyRamToFlash(0x0003C000+offset,(uint32_t*)*src,length*sizeof(uint8_t));
	}
	*/
	uint8_t* tmp;
	uint8_t buf[256];
	Chip_EEPROM_Write(offset, *src, length*sizeof(uint8_t));
	for(uint32_t i=0;i*256<length;i++){
		tmp = (uint8_t*)((uint32_t)0x0003C000+(offset&0xFFFFFF00)+i*0x100);
		memcpy(buf, tmp, 256); //copy page data
		if(offset%0x100+length-i*0x100>=0x100){
			if(i==0) memcpy(&buf[offset%256], *src, (256-(offset%256))*sizeof(uint8_t)); //insert source data
			else memcpy(buf, *src+(256-offset%256)+256*(i-1), 256*sizeof(uint8_t));
		}else{
			if(i==0) memcpy(&buf[offset%256], *src, length*sizeof(uint8_t)); //insert source data
			else memcpy(buf, *src+(256-offset%256)+256*(i-1), ((length-256*(i-1))-(offset%256))*sizeof(uint8_t));
		}
		//Chip_EEPROM_Write(offset, *src, length*sizeof(uint8_t));
		Chip_IAP_PreSectorForReadWrite(0x3C+((offset&0xFFFFFF00)+i*0x100)/0x1000, 0x3C+((offset&0xFFFFFF00)+i*0x100)/0x1000);
		Chip_IAP_ErasePage(0x3C0+((offset&0xFFFFFF00)+i*0x100)/0x100, 0x3C0+((offset&0xFFFFFF00)+i*0x100)/0x100);
		Chip_IAP_PreSectorForReadWrite(0x3C+((offset&0xFFFFFF00)+i*0x100)/0x1000, 0x3C+((offset&0xFFFFFF00)+i*0x100)/0x1000);
		Chip_IAP_CopyRamToFlash(0x0003C000+((offset&0xFFFFFF00)+i*0x100),(uint32_t*)buf,256);
	}
	/* exit critical section */
	NVIC_EnableIRQ(USB0_IRQn);
}

void msc_read(uint32_t offset, uint8_t** dst, uint32_t length, uint32_t high_offset){
	//TODO: msc read stub
	void* src;
	/* enter critical section */
	NVIC_DisableIRQ(USB0_IRQn);
	//Chip_EEPROM_Read(offset, *dst, length*sizeof(uint8_t));
	//Chip_IAP_PreSectorForReadWrite(0x3C+offset/0x1000, 0x3C+(offset+length)/0x1000);
	src = (void*)((uint32_t)0x0003C000+offset);
	memcpy(*dst, src, length*sizeof(uint8_t));
	/* exit critical section */
	NVIC_EnableIRQ(USB0_IRQn);
}

ErrorCode_t msc_verify(uint32_t offset, uint8_t buf[], uint32_t length, uint32_t high_offset){
	//TODO msc verify stub
	uint8_t tmp[length];
	void* src;
	/* enter critical section */
	NVIC_DisableIRQ(USB0_IRQn);
	//Chip_EEPROM_Read(offset, tmp, length*sizeof(uint8_t));
	//Chip_IAP_PreSectorForReadWrite(0x3C+offset/0x1000, 0x3C+(offset+length)/0x1000);
	src = (void*)((uint32_t)0x0003C000+offset);
	memcpy(tmp, src, length*sizeof(uint8_t));
	/* exit critical section */
	NVIC_EnableIRQ(USB0_IRQn);
	if(memcmp(buf, tmp, length*sizeof(uint8_t))==0) return LPC_OK;
	else return ERR_FAILED;
}

bool IsConnected() {
	return g_vCOM.tx_flags & VCOM_TX_CONNECTED;
}

static void ReadUpData() {
	uint8_t buf[RxTempSize];
	uint32_t len;
	//uint32_t idx;
	while ((len = vcom_bread(buf, RxTempSize)) > 0) {
		RingBuffer_InsertMult(&RxBuf,buf,len);
	}
}

uint32_t GetDepth(){
	return RingBuffer_GetCount(&RxBuf);
}
bool IsEmpty(){
	ReadUpData();
	return RingBuffer_IsEmpty(&RxBuf);
}

char ReadByte() {
	ReadUpData();
	char c;
	RingBuffer_Pop(&RxBuf,&c);
	return c;
}

string Read() {
	string s = "";
	char c;
	ReadUpData();
	while (!IsEmpty()) {
		RingBuffer_Pop(&RxBuf,&c);
		s+=c;
	}
	return s;
}

void Claer(){
	RingBuffer_Flush(&RxBuf);
}

bool IsBusy(){
	VCOM_DATA *pVcom = &g_vCOM;
	return pVcom->tx_flags & VCOM_TX_BUSY;
}

void Flush(){
	//前提としてまだオーバーフローしていないこと
	WriteDirect((uint8_t*)TxBuf,TxPos);//TxBufは先でコピーされる
	TxPos=0;
}

void Write(const char* byte,size_t size){
	size_t cs=0;
	size_t ncs;
	if (TxPos+size-1>TxBufferSize){
		cs= TxBufferSize-TxPos-1;//seek分
		memcpy(&TxBuf[TxPos],byte,cs);
		Flush();
	}

	ncs=min(size-cs,TxBufferSize);//コピーする分
	memcpy(&TxBuf[TxPos],&byte[cs],ncs);
	TxPos+=ncs;
	if (TxPos>TxBufferLimit){
		Flush();
	}

}

//割り込み
extern "C" void USB_IRQHandler(void) {
	USBD_API->hw->ISR(g_hUsb);
}

}

}
