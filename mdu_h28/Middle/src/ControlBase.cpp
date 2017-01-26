#include <ControlBase.h>

namespace Middle {
namespace Control {

uint32_t frequency = 1;	//周波数(非ゼロ)

uint32_t GetFrequency() {
	return frequency;
}

const char * const GetName(ControlMode c) {
	switch (c) {
	case ControlMode::None:
		return "None";
	case ControlMode::PID:
		return "PID";
		/*case ControlMode::Trapezium:
		 return "Trapezium";*/
	default:
		return "Error";
	}
}



ControlInterface::ControlInterface(ControlMode m) :
		mode(m) {
}

ControlInterface::~ControlInterface() {
}

}
}
