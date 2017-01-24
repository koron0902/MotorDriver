/*
 * QEI.h
 *
 *  Created on: 2016/11/05
 *      Author: TERU
 */

#ifndef DRIVER_QEI_H_
#define DRIVER_QEI_H_

#include <type.hpp>

namespace Device {
namespace QEI{

using namespace common;

void Init();
void DirInv(bool dir=false);
bool GetDirection();

void SetFilter(uint32_t clock);

void SetHandler(const callback_t&,uint8_t Priority=254);

void SetPosition(uint32_t);
uint32_t GetPosition();
uint32_t GetIndex();

void SetTimer(uint32_t clock);
uint32_t GetVelcoity();

extern volatile uint32_t* QEIVel;

}

} /* namespace Device */

#endif /* DRIVER_QEI_H_ */
