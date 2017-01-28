/*
 * QEI.h
 *
 *  Created on: 2016/11/05
 *      Author: TERU
 */

#ifndef DRIVER_QEI_H_
#define DRIVER_QEI_H_

#include <stdint.h>
#include <type.hpp>
#include <functional>


namespace Device {
namespace QEI{



void Init();
void Init(uint32_t clock);
void DirInv(bool dir=false);
bool GetDirection();

void SetFilter(uint32_t clock);

void SetCallBack();//実装予定

void SetPosition(uint32_t);
uint32_t GetPosition();
uint32_t GetIndex();

void SetTimer(uint32_t clock);
uint32_t GetVelcoity();
int32_t GetPulseCount();
void EnableInt();
extern volatile uint32_t* QEIVel;

}

} /* namespace Device */

#endif /* DRIVER_QEI_H_ */
