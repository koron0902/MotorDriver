/*
 * Timer.h
 *
 *  Created on: 2016/11/08
 *      Author: TERU
 */

#ifndef DEVICE_TIMER_H_
#define DEVICE_TIMER_H_

#include <functional>
#include <stdint.h>


namespace Device{
namespace Timer{
using clock_t = uint64_t;

void Init();

void SetAction(uint8_t ch,uint32_t hz,const std::function<void(void)>& action=nullptr);

void ResetSystemTimer();
clock_t GetSystemTime();//起動してからの大まかな目安
void Delay(clock_t);
}
}

#endif /* DEVICE_TIMER_H_ */
