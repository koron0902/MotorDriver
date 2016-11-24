
#pragma once
#include <fix.hpp>
namespace Device {

namespace ADC{
using namespace common;
uint16_t GetAmpA();
uint16_t GetAmpB();
uint16_t GetAmpC();

fix32 GetVlot();
fix32 GetVlotA();
fix32 GetVlotB();
fix32 GetVlotC();

void Init();

void Trigger();

}

} /* namespace Device */
