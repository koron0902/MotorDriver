
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
<<<<<<< HEAD

fix32 GetVolt();
fix32 GetVoltA();
fix32 GetVoltB();
fix32 GetVoltC();
=======
*/
fix32 GetVolt();
fix32 GetVoltU();
fix32 GetVoltV();
fix32 GetVoltW();

fix32 GetAmpU();
fix32 GetAmpV();
fix32 GetAmpW();

fix32 GetAccount();//CPU占有率

void Init();

void Trigger();



}

} /* namespace Device */
