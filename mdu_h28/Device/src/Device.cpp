#include <ADC.hpp>
#include <chip.hpp>
#include <Device.hpp>
#include <Port.hpp>
#include <PWM.hpp>
#include <QEI.hpp>
#include <Storage.hpp>
#include <Timer.hpp>
#include <Uart.hpp>
#include <USB.hpp>
#include <SPI.hpp>
#include <SysTick.hpp>
#include <HoleSensor.hpp>
#include <INT.hpp>

namespace Device {

void Init(){
	//using module supply clock
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();
	Tick::Init();
	//Each Function Initialize
	Port::Init();
	USB::Init();

	Storage::Init();
	Timer::Init();
	PWM::Init();
	Uart::Init();
	QEI::Init(1000);
	ADC::Init();
	SPI::Init();
	INT::Init();

}





} /* namespace Device */
