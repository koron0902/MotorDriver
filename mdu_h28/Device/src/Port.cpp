/*
 * Port.cpp
 *
 *  Created on: 2016/11/04
 *      Author: TERU
 */

#include <chip.h>
#include <Port.hpp>

namespace Device {
namespace Port {

//About LED
//const PortData LED0{1,3};//Note　代替
const PortData LED1{1, 8 };
const PortData LED2{1, 4 };
const PortData LED3{1, 5 };
//About SW
const PortData Func{1, 1 };
const PortData ISP0{1, 9 };
const PortData ISP1{1, 11 };
const PortData RST{0, 21 };
const PortData SC0{0, 24 };
const PortData SC1{1, 10 };
//About DRV(PWM)
const PortData PWMAH{0, 14 };
const PortData PWMAL{0, 5 };
const PortData PWMBH{0, 4 };
const PortData PWMBL{0, 3 };
const PortData PWMCH{0, 2 };
const PortData PWMCL{0, 1 };
const PortData PWMEN{0, 6 };
//About DRV(SPI)
const PortData SDI{0, 27 };
const PortData SDO{0, 28 };
const PortData SCK{0, 26 };
const PortData CS{0, 29 };
const PortData FALUT{1, 0 };
//About DRV(Analog)
const AnalogData SA_A{0, 31, 0, 9 };
const AnalogData SA_B{0, 0, 0, 10 };
const AnalogData SA_C{0, 30, 0, 11 };
const AnalogData SV_A{1, 2, 1, 4 };
const AnalogData SV_B{1, 4, 1, 10 }; //代替
const AnalogData SV_C{0, 11, 1, 3 };
const AnalogData SV{0, 10, 1, 2 };
const PortData SV_B_Alt{0, 12 }; //開放する(NC化する)

//About DRV(Others)
const PortData WAKE{0, 7 };
const PortData PWRGD{0, 25 };

//Communication
const PortData Tx{0, 18 };
const PortData Rx{0, 13 };
//他にも二個ピンがあるが使わないので定義しない。

//QEI
const PortData QEI_X{0, 22 };
const PortData QEI_Y{0, 23 };
const PortData QEI_Z{1, 7 };

//USB
const PortData UID{1, 6 };

//設定用補助関数群
static void SetDin(PortData data) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, data.port, data.pin,
			IOCON_DIGMODE_EN | IOCON_MODE_PULLUP);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO,data.port,data.pin);
}

static void SetDout(PortData data) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, data.port, data.pin, IOCON_MODE_INACT |IOCON_DIGMODE_EN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,data.port,data.pin);
	Chip_GPIO_SetPinState(LPC_GPIO, data.port, data.pin, true);
}

static void SetFuncMove(PortData data, CHIP_SWM_PIN_MOVABLE func) {
	Chip_SWM_MovablePortPinAssign(func, data.port, data.pin);
}


static void SetFuncFix(CHIP_SWM_PIN_FIXED_T pin) {
	Chip_SWM_FixedPinEnable(pin, true);
}

static void SetOpen(PortData data) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, data.port, data.pin, IOCON_ADMODE_EN); //アナログ化することで解放にする。
}

static void SetAnalog(AnalogData data) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, data.port, data.pin, IOCON_ADMODE_EN);
}

void Init() {
	//supply clock
	Chip_SWM_Init();
	Chip_GPIO_Init(LPC_GPIO);
	Chip_GPIOGP_Init(LPC_GPIOGROUP);
	//LED
	//SetDout(LED0);//Note 代替
	SetDout(LED1);
	Set(LED1,true);
	SetDout(LED2);
	SetDout(LED3);

	//SW
	SetDin(Func);
	SetDin(ISP0);
	SetDin(ISP1);
	SetDin(RST);
	SetDin(SC0);
	SetDin(SC1);

	//DRV(PWM)
	SetDout(PWMAH);
	SetFuncFix(SWM_FIXED_SCT1_OUT5);
	SetDout(PWMAL);
	//SetFuncMove(PWMAL,SWM_SCT1_OUT2_O);
	SetDout(PWMBH);
	//SetFuncMove(PWMBH,SWM_SCT1_OUT1_O);
	SetDout(PWMBL);
	//SetFuncFix(SWM_FIXED_SCT1_OUT4);
	SetDout(PWMCH);
	//SetFuncFix(SWM_FIXED_SCT1_OUT3);
	SetDout(PWMCL);
	//SetFuncMove(SWM_SCT1_OUT0_O);
	SetDout(PWMEN);

	//DRV(SPI)
	SetDout(SDO);
	SetFuncMove(SDO, SWM_SPI0_MOSI_IO);
	SetDin(SDI);
	SetFuncMove(SDI, SWM_SPI0_MISO_IO);
	SetDout(SCK);
	SetFuncMove(SCK, SWM_SPI0_SCK_IO);
	SetDout(CS);
	SetFuncMove(CS, SWM_SPI0_SSELSN_0_IO);
	SetDin(FALUT);

	//あとで割り込み対応に
	//DRV(analog)
	SetAnalog(SA_A);
	SetFuncFix(SWM_FIXED_ADC0_9);
	SetAnalog(SA_B);
	SetFuncFix(SWM_FIXED_ADC0_10);
	SetAnalog(SA_C);
	SetFuncFix(SWM_FIXED_ADC0_11);
	SetAnalog(SV_A);
	SetFuncFix(SWM_FIXED_ADC1_4);
	SetAnalog(SV_B);
	SetFuncFix(SWM_FIXED_ADC1_5);//NOTE 代替ピン
	SetAnalog(SV_C);
	SetFuncFix(SWM_FIXED_ADC1_3);
	SetAnalog(SV);
	SetFuncFix(SWM_FIXED_ADC1_2);
	SetOpen(SV_B_Alt);	//Note 代替のため

	//DRV(Other)
	SetDout(WAKE);
	SetDout(PWRGD);

	//Communication
	SetDout(Tx);
	SetFuncMove(Tx, SWM_UART0_TXD_O);
	SetDin(Rx);
	SetFuncMove(Rx, SWM_UART0_RXD_I);

	//QEI
	SetDin(QEI_X);
	SetFuncMove(QEI_X, SWM_QEI0_PHA_I);
	SetDin(QEI_Y);
	SetFuncMove(QEI_Y, SWM_QEI0_PHB_I);
	SetDin(QEI_Z);
	SetFuncMove(QEI_Z, SWM_QEI0_IDX_I);

	//USB
	SetDin(UID);
	SetFuncMove(UID, SWM_USB_VBUS_I);

}

void Set(PortData data, bool fag) {
	Chip_GPIO_SetPinState(LPC_GPIO, data.port, data.pin, fag);
}

bool Get(PortData data) {
	return Chip_GPIO_GetPinState(LPC_GPIO, data.port, data.pin);
}

void Toggle(PortData data){
	Chip_GPIO_SetPinToggle(LPC_GPIO,data.port,data.pin);
}

}
} /* namespace Device */
