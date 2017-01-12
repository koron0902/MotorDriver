#pragma once

#include <unit.hpp>
namespace Device{

//RTOSを組み込み時は無効化すること

namespace Tick{
using namespace common;
constexpr static uint32_t SampleRate=1_KHz;
/*システムモニタ用タイマーの初期化*/
void Init();

uint64_t Tick();

uint64_t TickUs();//マイクロ秒単位
uint64_t TickMs();//ミリ秒
uint64_t TickS();
void DelayUs(uint64_t);//マイクロ秒単位で待つ
void DelayMs(uint64_t);//ミリ秒単位で待つ
}
}
