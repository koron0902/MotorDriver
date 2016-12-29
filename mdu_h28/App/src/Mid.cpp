#include <Mid.hpp>
#include <Motor.hpp>
#include <stdlib.h>
#include <text.hpp>
#include <DRV.hpp>
#include <Trapezium.hpp>
#include <PIDControl.hpp>
using namespace Middle;
using namespace App::File;
using namespace common;

namespace App {
namespace Mid{

Directory* Create(){
	auto *mid=Directory::Create("mid");
	mid->Add(Execute::Create("duty",Duty));
	mid->Add(Execute::Create("free",Free));
	mid->Add(Execute::Create("lock",Lock));
	mid->Add(CreateDRV());
	mid->Add(CreateTrap());
	mid->Add(CreatePID());
	return mid;
}

Directory* CreateDRV(){
	auto* drv = Directory::Create("drv");
	drv->Add(FileProperty::Create("gain", *Middle::DRV::GetGain, *Middle::DRV::SetGain));

	return drv;
}

Directory* CreateTrap(){
	auto* trap = Directory::Create("trap");
	trap->Add(FileProperty::Create("duty", *Controller::Trapezium::GetNowDuty, *Controller::Trapezium::SetTargetDuty));
	trap->Add(FileProperty::Create("step", *Controller::Trapezium::GetStep, Controller::Trapezium::SetStep));

	return trap;
}

Directory* CreatePID(){
	auto* pid = Directory::Create("pid");
	pid->Add(FileFix::Create("duty", &(Controller::PID::LastDuty)));

	return pid;
}

std::string Duty(const common::ShellParameter& arg){
	if (arg.size()>1){
		Motor::SetDuty(common::ToFix(arg[1]));
	}else{
		Motor::Lock();
	}
	return "";
}
std::string Free(const common::ShellParameter&){
	Motor::Free();
	return "";
}

std::string Lock(const common::ShellParameter&){
	Motor::Lock();
	return "";
}





}

} /* namespace App */
