/*
 * Timer.h
 *
 *  Created on: 2016/11/08
 *      Author: TERU
 */

#ifndef DEVICE_TIMER_H_
#define DEVICE_TIMER_H_

#include <type.hpp>
#include <stdint.h>


namespace Device{
namespace Timer{
using namespace common;
using clock_t = uint64_t;

enum class TimerID: uint32_t{
Timer0,	//MotorControlで使用する。
Timer1,
Timer2,
Timer3,
};

void Init();
//割り込みハンドラに関する関数
void SetHandler(const callback_t& callback,TimerID id,uint32_t period);
void SetHandler(callback_t&& callback,TimerID id,uint32_t period);
void KillHandler(TimerID id);
void ChangePeriod(TimerID id,uint32_t period);
//void SetAction(uint8_t ch,uint32_t hz,const std::function<void(void)>& action=nullptr);
//void SetAction(uint8_t ch,uint32_t hz,std::function<void(void)>&& action);
void ResetSystemTimer();
clock_t GetSystemTime();//起動してからの大まかな目安
void Delay(clock_t);
}
}

#endif /* DEVICE_TIMER_H_ */
