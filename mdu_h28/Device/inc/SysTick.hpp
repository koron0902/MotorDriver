#pragma once

#include <stdint.h>
#include <unit.hpp>
namespace Device{

//RTOSを組み込み時は無効化すること

namespace Tick{
using namespace common;
constexpr static uint32_t SampleRate=1_KHz;
/*システムモニタ用タイマーの初期化*/
void Init();

uint64_t Tick();
uint64_t TickSub();

}
}
