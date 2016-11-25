
#pragma once
#include <fix.hpp>
namespace Device {

namespace ADC{
using namespace common;

void SetAmpGain(const fix32& gain);
/*
uint16_t GetAmpA();
uint16_t GetAmpB();
uint16_t GetAmpC();
*/
fix32 GetVlot();
fix32 GetVlotU();
fix32 GetVlotV();
fix32 GetVlotW();

fix32 GetAmpU();
fix32 GetAmpV();
fix32 GetAmpW();

void Init();

void Trigger();



}

} /* namespace Device */
