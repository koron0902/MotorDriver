/*
 * ControllerBase.cpp
 *
 *  Created on: 2016/12/11
 *      Author: Keita
 */

#include <ControllerBase.hpp>
#include <Trapezium.hpp>
#include <PIDControl.hpp>
#include <Timer.hpp>
#include <motor.hpp>
#include <Port.hpp>
#include <XPort.hpp>
#include <configuration.hpp>
#include <QEI.hpp>

namespace Middle {
namespace Controller {
using namespace Device;

static ControlMode Mode;

void SwitchControlMode(ControlMode m) {
	if (Mode != m) {

		switch (m) {
		case ControlMode::Test: {
			Device::Port::Set(Device::Port::PWMEN, true);
			Device::Timer::SetAction(ControllerBase::mControllerTaskPriority, 1,
					nullptr);
			//Device::QEI::SetHandler(nullptr,PriorityControl);

			XPort::WriteLine("Succeeded in switching test mode");
			break;
		}
		case ControlMode::Trapezium: {
			Trapezium* trap = new Trapezium();
			Device::Port::Set(Device::Port::PWMEN, true);

			Controller::Trapezium::Reset();
			Device::Timer::SetAction(ControllerBase::mControllerTaskPriority,
					trap->GetFreq(), std::move(*trap));
			//QEI::SetHandler(std::move(*trap),PriorityControl);
			XPort::WriteLine("Succeeded in switching trap. control");
			break;
		}
		case ControlMode::PID: {
			PID* pid = new PID();
			Device::Port::Set(Device::Port::PWMEN, true);
			Controller::PID::Reset();
			Device::Timer::SetAction(ControllerBase::mControllerTaskPriority,
					pid->GetFreq(), std::move(*pid));
			//QEI::SetHandler(std::move(*pid),PriorityControl);
			XPort::WriteLine("Succeeded in switching pid control");
			break;
		}
		default: {
			XPort::WriteLine("No such as control mode");
			return;
		}
		}
		Mode = m;
	} else {
		XPort::WriteLine("Already running as requested mode");
	}

}

ControlMode GetMode() {
	return Mode;
}

const char * const GetName(ControlMode c) {
	switch (c) {
	case ControlMode::Test:
		return "Test";
	case ControlMode::PID:
		return "PID";
	case ControlMode::Trapezium:
		return "Trapezium";
	default:
		return "Error";
	}
}

ControllerBase::ControllerBase() :
		mFreq(FREQ_DEFAULT) {

}

ControllerBase::~ControllerBase() {

}

} /* namespace Controller */
} /* namespace Middle */
