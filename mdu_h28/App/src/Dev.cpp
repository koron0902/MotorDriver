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
	dir->Add(Integer::Create("qei", (int32_t*) QEI::QEIVel));

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
	dir->Add(File::CreateReadOnlyProperty("batt", [](){
		return ToStr(ADC::GetVlot());
	}));

	dir->Add(
			File::CreateReadOnlyProperty("vlot",
					[] () {
						return ToStr(ADC::GetVlotU())+" "+ToStr(ADC::GetVlotV())+" "+ToStr(ADC::GetVlotU());
					}));

	dir->Add(
			File::CreateReadOnlyProperty("amp",
					[] () {
						return ToStr(ADC::GetAmpU())+" "+ToStr(ADC::GetAmpV())+" "+ToStr(ADC::GetAmpU());
					}));

	return dir;
}

}
} /* namespace Device */
