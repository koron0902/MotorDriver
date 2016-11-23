#include <xport.hpp>
#include <USB.hpp>
#include <Uart.hpp>

namespace Middle {
namespace XPort {

using namespace Device;

void Init() {
	//ダミー
	;
}

bool IsEmpty() {
	return Uart::IsEmpty() && USB::IsEmpty();
}

void Write(const std::string& text) {
	Uart::Write(text);
	if (USB::IsConnected()) {
		USB::Write(text);
	}
}

void Write(char c){
	Uart::Write(c);
	if (USB::IsConnected()) {
		USB::Write(c);
	}
}


void Claer(){
	Uart::Claer();
	USB::Claer();
}

void Flush(){
	if (USB::IsConnected()){
	USB::Flush();
	}
}

}
}
