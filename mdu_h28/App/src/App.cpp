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
#define forever() for(;;)

using namespace Device;
using namespace std;
using namespace App::File;

namespace App {

static bool usb_flag = false;
static string buffer_uart;
static string buffer_usb;

void Init() {
	Shell::Init();
	buffer_uart.reserve(32);
	buffer_usb.reserve(32);
}

void CommandLine() {
	char c;
	while (!Uart::IsEmpty()) {
		c = Uart::ReadByte();
		Port::Toggle(Port::LED3);
		if (c != common::newline) {
			buffer_uart += c;

		} else {
			string temp = Shell::Call(buffer_uart);
			if (temp != "") {
				Uart::WriteLine();
			}
			buffer_uart = "";
		}
	}

	if (!usb_flag && USB::IsConnected()) {
		//USB::Write((uint8_t*) "Connected!!\r\n", 15);
		usb_flag = true;
	}

	if (usb_flag) {
		while (!USB::IsEmpty()) {
			c = USB::ReadByte();
			Port::Toggle(Port::LED3);
			if (c != common::newline) {
				buffer_usb += c;
			} else {
				string temp = Shell::Call(buffer_usb);
				if (temp != "") {
					USB::WriteLine(temp);
					USB::Flush();
				}
				buffer_usb = "";
			}
		}
	}
}

void Run() {
	string buf;

	USB::WriteLine("linked　MDU(prototype)");
	USB::WriteLine(current->GetAllName());
	for(;;) {

		CommandLine();
		Middle::DRV::Update();
		Device::ADC::Trigger();

		//for(volatile int i = 0;i < 5000; i++);
		//Device::QEI::GetVelcoity();
	}
	//can't reach here
}

} /* namespace Device */
