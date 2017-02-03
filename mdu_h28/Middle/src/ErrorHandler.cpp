#include "ErrorHandler.hpp"

#include <Port.hpp>
#include <xport.hpp>
#include <stdint.h>
#include <bits.hpp>
#include <chip.hpp>

using namespace Device;
using namespace common;

namespace Middle {
namespace Error {

extern "C" void HardFault_Handler(void) {
	Port::Set(Port::PWMEN, false);
	//大抵NULLの参照ミスである。
	volatile uint32_t cnt;
	//目でわかるようにLEDを点滅させる。
	for  (cnt=0xFFFFFF;cnt!=0;cnt--){
		Port::Set(Port::LED2,cnt&0x20000);
		Port::Set(Port::LED3,cnt&0x20000);
	}
	//気づいたころに再起動をかける
	NVIC_SystemReset();
}

}
}
