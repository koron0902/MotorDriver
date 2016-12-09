/*
 * Port.h
 *
 *  Created on: 2016/11/04
 *      Author: TERU
 */

#ifndef DRIVER_PORT_H_
#define DRIVER_PORT_H_

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
};

struct AnalogData{
	port_t port;
	pin_t pin;
	module_t module;
	id_t id;
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




}
}

#endif /* DRIVER_PORT_H_ */
