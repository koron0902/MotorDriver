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
namespace Dev{

Directory* Create(){
	Directory* dir=Directory::Create("dev");
	dir->Add(CreateUart());
	dir->Add(CreateADC());
	dir->Add(FileInt32::Create("qei", (int32_t*)QEI::QEIVel));

	return dir;
}

Directory* CreateUart() {
	Directory* uart = Directory::Create("uart");

	uart->Add(Execute::Create("write", [](const vector<string>& arg)->string {
		if (arg.size()>1); {
			Uart::Write(arg[1]);
		}
		return "";
	}));
	
	uart->Add(Execute::Create("band", [](const vector<string>& arg)->string {
		if (arg.size()>2){
			//int band=atoi(arg[2].c_str());
			//if (band!=0){
				//後日実装予定
			//}else{

			//}
		}
		return "Standby";
	}));

	return uart;
}

File::Directory* CreateADC(){
	Directory* dir = Directory::Create("ad");

	dir->Add(Execute::Create("dump",[](const vector<string>& dummy)->string{

		string out;
		out.reserve(64);
		out=ToStr(ADC::GetVlot())+",";
		out+=ToStr(ADC::GetVlotA())+",";
		out+=ToStr(ADC::GetVlotB())+",";
		out+=ToStr(ADC::GetVlotC());

		return out;
	}));
	Middle::XPort::Write("pass");
	return dir;
}






}
} /* namespace Device */
