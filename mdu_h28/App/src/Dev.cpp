#include <Dev.hpp>
#include <Uart.hpp>
using namespace App::File;
using namespace std;
using namespace Device;

namespace App {

Directory* CreateDev() {
	Directory* dev = Directory::Create("dev");
	dev->Add(CreateUart());
	return dev;
}

Directory* CreateUart() {
	Directory* uart = Directory::Create("uart");

	uart->Add(Execute::Create("write", [](const vector<string>& arg)->string {
		if (arg.size()>2); {
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

} /* namespace Device */
