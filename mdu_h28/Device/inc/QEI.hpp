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



void SetPosition(uint32_t);
int32_t GetPositionRaw();
int64_t GetPosition();
int32_t GetIndex();
int32_t GetVelcoity();





//extern volatile uint32_t* QEIVel;

//QEIの信号線の状態に関して(接続確認用)
std::string GetPulseName();

void SetHandler(const callback_t&,uint8_t Priority=254);







}

} /* namespace Device */

#endif /* DRIVER_QEI_H_ */
