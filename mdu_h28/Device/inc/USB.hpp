#pragma once

#include <error.hpp>
#include <stdint.h>
#include <string>
#include <vector>
#include "usb/app_usbd_cfg.h"
#include <text.hpp>
#include <stddef.h>
namespace Device {

namespace USB {

#define VCOM_RX_BUF_SZ      512
#define VCOM_TX_CONNECTED   _BIT(8)		/* connection state is for both RX/Tx */
#define VCOM_TX_BUSY        _BIT(0)
#define VCOM_RX_DONE        _BIT(1)
#define VCOM_RX_BUF_FULL    _BIT(2)
#define VCOM_RX_BUF_QUEUED  _BIT(3)
#define VCOM_RX_DB_QUEUED   _BIT(4)

/**
 * Structure containing Virtual Comm port control data
 */

constexpr size_t RxBufferSize=128;//二次バッファの大きさ(受信)
constexpr int TxBufferSize=256;
constexpr int TxBufferLimit=196;
constexpr size_t RxTempSize=32;//二次バッファへ移動する際のバッファの大きさ(スタックに乗る)

// TODO USB header stub
void Init();

/**
 * @brief	Virtual com port buffered read routine
 * @param	pBuf	: Pointer to buffer where read data should be copied
 * @param	buf_len	: Length of the buffer passed
 * @return	Return number of bytes read.
 */
uint32_t vcom_bread (uint8_t *pBuf, uint32_t buf_len);

/**
 * @brief	Virtual com port read routine
 * @param	pBuf	: Pointer to buffer where read data should be copied
 * @param	buf_len	: Length of the buffer passed
 * @return	Always returns LPC_OK.
 */
ErrorCode_t vcom_read_req (uint8_t *pBuf, uint32_t buf_len);

/**
 * @brief	Gets current read count.
 * @return	Returns current read count.
 */
uint32_t vcom_read_cnt(void);

/**
 * @brief	Check if Vcom is connected
 * @return	Returns non-zero value if connected.
 */
/*
static INLINE uint32_t Connected(void) {
	return g_vCOM.tx_flags & VCOM_TX_CONNECTED;
}
*/
/**
 * @brief	Virtual com port write routine
 * @param	pBuf	: Pointer to buffer to be written
 * @param	buf_len	: Length of the buffer passed
 * @return	Number of bytes written
 */
uint32_t vcom_write (const uint8_t *pBuf, uint32_t buf_len);

/**
 * @}
 */

//Mass storage class declaration

ErrorCode_t msc_init(USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc, USBD_API_INIT_PARAM_T *pUsbParam);
void msc_write(uint32_t offset, uint8_t** src, uint32_t length, uint32_t high_offset);
void msc_read(uint32_t offset, uint8_t** dst, uint32_t length, uint32_t high_offset);
ErrorCode_t msc_verify(uint32_t offset, uint8_t buf[], uint32_t length, uint32_t high_offset);

//自作関数群
bool IsConnected();
uint32_t GetDepth();//受信文字数
bool IsEmpty();

char ReadByte();
std::string Read();
std::string ReadLine();

void Claer();
bool IsBusy();//送信中？

void Write(const uint8_t* byte,size_t size);
static inline void Write(const std::string& text){
	Write((uint8_t*)text.data(),text.length());
}

static inline void WriteLine(const std::string& text){
	 Write(text+common::newline);
}

static inline void Write(char c){//非推奨
	 Write((uint8_t*)&c,sizeof(c));
}

bool IsExist(char);
static inline bool IsLine(){
	return IsExist(common::newline);
}

}

}

