#include <ADC.hpp>
#include <chip.h>
#include <Device.hpp>
#include <Port.hpp>
#include <PWM.hpp>
#include <QEI.hpp>
#include <Storage.hpp>
#include <Timer.hpp>
#include <Uart.hpp>
#include <USB.hpp>
#include <SPI.hpp>

namespace Device {

void Init(){
	//using module supply clock
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();

	//Each Function Initialize
	Port::Init();

	Storage::Init();
	Timer::Init();
	PWM::Init();
	Uart::Init();
	QEI::Init();
	ADC::Init();
	USB::Init();
	SPI::Init();
}





} /* namespace Device */
