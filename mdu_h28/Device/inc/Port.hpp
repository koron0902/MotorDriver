#ifndef DRIVER_PORT_H_
#define DRIVER_PORT_H_
#include <swm_data.hpp>
#include <stdint.h>

namespace Device {
//using phy_t=uint8_t;
using port_t=uint8_t;
using pin_t =uint8_t;
using module_t =uint8_t;
using id_t =uint8_t;


namespace Port{

struct PortData{
	port_t port;
	pin_t pin;

	void Set(bool)const;
	bool Get()const;
	void Toggle()const;
	//設定用関数
	const PortData& Din()const;
	const PortData& Dout()const;
	//const PortData& Analog()const;
	const PortData& Open()const;
	const PortData& Move(CHIP_SWM_PIN_MOVABLE_T func)const;
	const PortData& Fix(CHIP_SWM_PIN_FIXED_T fix)const;
};

struct AnalogData{
	port_t port;
	pin_t pin;
	module_t module;
	id_t id;
	const AnalogData& Init(CHIP_SWM_PIN_FIXED_T)const;
	PortData GetPortData()const{
		return PortData{port,pin};
	}

};

//About LED
//extern const PortData LED0;//他の機能に使用している
extern const PortData LED1;
extern const PortData LED2;
extern const PortData LED3;
//About SW
extern const PortData Func;
extern const PortData ISP0;
extern const PortData ISP1;
extern const PortData RST;
extern const PortData SC0;
extern const PortData SC1;
//About DRV(PWM)
extern const PortData PWMAH;
extern const PortData PWMAL;
extern const PortData PWMBH;
extern const PortData PWMBL;
extern const PortData PWMCH;
extern const PortData PWMCL;
extern const PortData PWMEN;
//About DRV(SPI)
extern const PortData SDO;
extern const PortData SDI;
extern const PortData SCK;
extern const PortData CS;
extern const PortData FALUT;
//About DRV(Analog)
extern const AnalogData SA_A;
extern const AnalogData SA_B;
extern const AnalogData SA_C;
extern const AnalogData SV_A;
extern const AnalogData SV_B;
extern const AnalogData SV_C;
extern const AnalogData SV;

//About DRV(Others)
extern const PortData WAKE;
extern const PortData PWRGD;

//Communication
extern const PortData Tx;
extern const PortData Rx;

//QEI
extern const PortData QEI_X;
extern const PortData QEI_Y;
//extern const PortData QEI_Z;

//USB
extern const PortData UID;

//Hole Sensors
extern const PortData HoleU;
extern const PortData HoleV;
extern const PortData HoleW;

void Init();

void Set(PortData,bool);
bool Get(PortData);
void Toggle(PortData);
/*
 * 設定用関数[非推奨]
 */
void SetDin(PortData data);
void SetDout(PortData data);
void SetFuncMove(PortData data, CHIP_SWM_PIN_MOVABLE_T func);
void SetFuncFix(CHIP_SWM_PIN_FIXED_T pin);
void SetOpen(PortData data) ;
void SetAnalog(AnalogData data) ;
}
}

#endif /* DRIVER_PORT_H_ */
