/*
 * configuration.cpp
 *
 *  Created on: 2017/01/20
 *      Author: hamus
 */
#include <unit.hpp>
#include <configuration.hpp>

namespace common{
//割り込み用優先度定数

//未定義(あとで考える。)
const uint8_t PrioritySCT1=0xFE;
const uint8_t PriorityMRT=0xFE;
const uint8_t PriorityADC=0xFE;

//システム系
const uint8_t PriorityDRVFault=0x0E;
const uint8_t PrioritySysTick=0x0F;
//角度制御関係
const uint8_t PriorityHoleSensor=0x10;
//周期制御
const uint8_t PriorityControl=0x20;
//通信系
const uint8_t PriorityUSB=0xA0;
const uint8_t PriorityUart=0xAF;


//周期
const uint32_t CyclePWM	= 30_KHz;
const uint32_t CycleControl = 15_KHz;
const uint32_t Divider=CyclePWM/CycleControl;

}
