#include <App.hpp>
#include <DRV.hpp>
#include <ADC.hpp>
#include <QEI.hpp>
//#include <File.hpp>
#include <Port.hpp>
#include <string.h>
#include <Shell.hpp>
#include <Uart.hpp>
#include <USB.hpp>
#include <text.hpp>
#include <PIDControl.hpp>
#include <Trapezium.hpp>
#include <Timer.hpp>
#include <XPort.hpp>
#include <error.hpp>

#define forever() for(;;)

using namespace Device;
using namespace std;
using namespace Middle;

namespace App {

Middle::Controller::Trapezium* trap;
void Init() {
	Shell::Init();
	trap = new Middle::Controller::Trapezium();
	Device::Timer::SetAction(1, trap->GetFreq(), std::move(*trap));
}

void CommandLine() {

	string buf;
	buf.reserve(128);
	auto call = [&buf]() {

		while (XPort::IsLine()) {
			//int code;
			buf = XPort::ReadLine();
			/*code=*/Shell::Call(buf);

		}
		XPort::Flush();
	};


	auto temp = XPort::GetPort();
	XPort::SetPort(XPort::PortFlag::UART);
	call();
	if (XPort::IsEnableUSB()) {
		XPort::SetPort(XPort::PortFlag::USB);
		call();
	}
	XPort::SetPort(temp);
}

void Run() {
	string buf;
	forever() {

		CommandLine();
		Middle::DRV::Update();
		//Device::ADC::Trigger();
	}
	//can't reach here
}

} /* namespace Device */
