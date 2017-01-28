#include <Dev.hpp>
#include <Uart.hpp>
#include <ADC.hpp>
#include <text.hpp>
#include <xport.hpp>
#include <QEI.hpp>
using namespace App::File;
using namespace std;
using namespace Device;
using namespace common;
namespace App {
namespace Dev {

Directory* Create() {
	Directory* dir = Directory::Create("dev");
	dir->Add(CreateUart());
	dir->Add(CreateADC());
	dir->Add(CreateQEI());
	return dir;
}

Directory* CreateUart() {
	using iterator = std::vector<std::string>::iterator;
	Directory* uart = Directory::Create("uart");

	auto write = [](iterator begin,iterator end)->int {
		if (std::distance(begin,end)>=1) {
			begin++;
			Uart::Write(*begin);
		}
		return 0;
	};

	uart->Add(Execute<decltype(write)>::Create("write", write));

	return uart;
}


File::Directory* CreateADC() {
	Directory* dir = Directory::Create("adc");
	dir->Add(File::CreateReadOnlyProperty("batt", []() {
		return ToStr(ADC::GetVolt());
	}));

	dir->Add(
			File::CreateReadOnlyProperty("Volt",
					[] () {
						return ToStr(ADC::GetVoltU())+" "+ToStr(ADC::GetVoltV())+" "+ToStr(ADC::GetVoltU());
					}));

	dir->Add(
			File::CreateReadOnlyProperty("amp",
					[] () {
						return ToStr(ADC::GetAmpU())+" "+ToStr(ADC::GetAmpV())+" "+ToStr(ADC::GetAmpU());
					}));

	dir->Add(File::CreateReadOnlyProperty("count", []() ->string {
		return ToStr(ADC::GetAccount()*fix32::CreateInt(100));//百分率
	}));

	return dir;
}

File::Directory* CreateQEI(){
	auto* qei = Directory::Create("qei");

	qei->Add(File::CreateExecute("count", [](text_iterator, text_iterator)->int{
		Middle::XPort::WriteLine(common::ToStr(QEI::GetPulseCount()));

		return 0;
	}));

	qei->Add(File::CreateExecute("dir", [](text_iterator, text_iterator)->int{
		Middle::XPort::WriteLine(common::ToStr((uint32_t)QEI::GetDirection()));
		return 0;
	}));

	return qei;
}



}
} /* namespace Device */
