#include <chip.hpp>
#include "port.hpp"

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
const PortData QEI_X{0, 15 };
const PortData QEI_Y{0, 16 };
const PortData QEI_Z{1, 7 };

//Hole Sensor
const PortData HoleU{0, 22 };
const PortData HoleV{0, 23 };
const PortData HoleW{0, 24 };

//USB
const PortData UID{1, 6 };

//設定用補助関数群
/* 互換用に残しているが後々消す。*/
void SetDin(PortData data) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, data.port, data.pin,
	IOCON_DIGMODE_EN | IOCON_MODE_PULLUP);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, data.port, data.pin);
}

void SetDout(PortData data) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, data.port, data.pin,
	IOCON_MODE_INACT | IOCON_DIGMODE_EN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, data.port, data.pin);
	Chip_GPIO_SetPinState(LPC_GPIO, data.port, data.pin, true);
}

void SetFuncMove(PortData data, CHIP_SWM_PIN_MOVABLE func) {
	Chip_SWM_MovablePortPinAssign(func, data.port, data.pin);
}

void SetFuncFix(CHIP_SWM_PIN_FIXED_T pin) {
	Chip_SWM_FixedPinEnable(pin, true);
}

void SetOpen(PortData data) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, data.port, data.pin,
	IOCON_ADMODE_EN | IOCON_MODE_PULLDOWN); //アナログ化することで解放にする。
}

void SetAnalog(AnalogData data) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, data.port, data.pin,
	IOCON_ADMODE_EN | IOCON_MODE_PULLDOWN);
}

void Set(PortData data, bool fag) {
	Chip_GPIO_SetPinState(LPC_GPIO, data.port, data.pin, fag);
}

bool Get(PortData data) {
	return Chip_GPIO_GetPinState(LPC_GPIO, data.port, data.pin);
}

void Toggle(PortData data) {
	Chip_GPIO_SetPinToggle(LPC_GPIO, data.port, data.pin);
}

void PortData::Set(bool flag)const {
	Chip_GPIO_SetPinState(LPC_GPIO, port, pin, flag);
}

bool PortData::Get() const{
	return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
}

void PortData::Toggle() const{
	Chip_GPIO_SetPortToggle(LPC_GPIO, port, pin);
}

const PortData& PortData::Din()const {
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
	IOCON_DIGMODE_EN | IOCON_MODE_PULLUP);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	return *this;
}

const PortData& PortData::Dout()const {
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
	IOCON_MODE_INACT | IOCON_DIGMODE_EN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
	Chip_GPIO_SetPinState(LPC_GPIO, port, pin, true);
	return *this;
}

const PortData& PortData::Open()const {
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
	IOCON_ADMODE_EN | IOCON_MODE_PULLDOWN); //アナログ化することで解放にする。
	return *this;
}

const PortData& PortData::Move(CHIP_SWM_PIN_MOVABLE_T func)const{
	Chip_SWM_MovablePortPinAssign(func, port, pin);
	return *this;
}

const PortData& PortData::Fix(CHIP_SWM_PIN_FIXED_T fix)const{
	Chip_SWM_FixedPinEnable(fix, true);
	return *this;
}

const AnalogData& AnalogData::Init(CHIP_SWM_PIN_FIXED_T pin)const{
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
	IOCON_ADMODE_EN | IOCON_MODE_PULLDOWN);
	Chip_SWM_FixedPinEnable(pin, true);
	return *this;
}


void Init() {
	//supply clock
	Chip_SWM_Init();
	Chip_GPIO_Init(LPC_GPIO);
	//LED
	//SetDout(LED0);//Note 代替
	LED1.Dout().Set(true);
	LED2.Dout();
	LED3.Dout();

	//SW
	Func.Din();
	ISP0.Din();
	ISP1.Din();
	RST.Din();
	SC0.Din();
	SC1.Din();

	//DRV(PWM)
	PWMAH.Dout().Fix(SWM_FIXED_SCT1_OUT5);
	PWMAL.Dout();
	//SetFuncMove(PWMAL,SWM_SCT1_OUT2_O);
	PWMBH.Dout().Move(SWM_SCT1_OUT1_O);
	//SetFuncMove(PWMBH,SWM_SCT1_OUT1_O);
	PWMBL.Dout();
	//SetFuncFix(SWM_FIXED_SCT1_OUT4);
	PWMCH.Dout().Fix(SWM_FIXED_SCT1_OUT3);
	//SetFuncFix(SWM_FIXED_SCT1_OUT3);
	PWMCL.Dout();
	//SetFuncMove(SWM_SCT1_OUT0_O);
	PWMEN.Dout();

	//DRV(SPI)
	SDO.Dout().Move(SWM_SPI0_MOSI_IO);
	SDI.Din().Move( SWM_SPI0_MISO_IO);
	SCK.Dout().Move( SWM_SPI0_SCK_IO);
	CS.Dout().Move( SWM_SPI0_SSELSN_0_IO);
	FALUT.Din();

	//あとで割り込み対応に
	//DRV(analog)
	SA_A.Init(SWM_FIXED_ADC0_9);
	SA_B.Init(SWM_FIXED_ADC0_10);
	SA_C.Init(SWM_FIXED_ADC0_11);
	SV_A.Init(SWM_FIXED_ADC1_4);
	SV_B.Init(SWM_FIXED_ADC1_5);	//NOTE 代替ピン
	SV_C.Init(SWM_FIXED_ADC1_3);
	SV.Init(SWM_FIXED_ADC1_2);
	SV_B_Alt.Open();	//Note 代替のため

	//DRV(Other)
	WAKE.Dout();
	PWRGD.Dout();

	//Communication
	Tx.Dout().Move( SWM_UART0_TXD_O);
	Rx.Din().Move( SWM_UART0_RXD_I);

	//QEI
	QEI_X.Din().Move( SWM_QEI0_PHA_I);
	QEI_Y.Din().Move(SWM_QEI0_PHB_I);
	//SetDin(QEI_Z);
	//SetFuncMove(QEI_Z, SWM_QEI0_IDX_I);

	//USB
	UID.Din().Move( SWM_USB_VBUS_I);

}

}
} /* namespace Device */
