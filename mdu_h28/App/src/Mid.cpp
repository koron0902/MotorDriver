#include <Mid.hpp>
#include <Motor.hpp>
#include <text.hpp>
#include <DRV.hpp>
#include <Trapezium.hpp>
#include <PIDControl.hpp>
#include <ControllerBase.hpp>
#include <type.hpp>
#include<File.hpp>
#include <xport.hpp>

using namespace Middle;
using namespace App::File;
using namespace common;

namespace App {
namespace Mid{
Directory* Create(){
	auto *mid=Directory::Create("mid");
	mid->Add(CreateBasic());
	mid->Add(CreateDRV());
	mid->Add(CreateTrap());
	mid->Add(CreatePID());
	mid->Add(CreateSwitch());
	return mid;
}

Directory* CreateDRV(){
	auto* drv = Directory::Create("drv");
	//drv->Add(File::CreateProperty("gain",*(Middle::DRV::GetGain),*(Middle::DRV::SetGain)));
	return drv;
}

Directory* CreateTrap(){
	auto* trap = Directory::Create("trap");
	trap->Add(File::Fix::Create("tDuty", &(Controller::Trapezium::mMotorState.mTargetDuty)));
	trap->Add(File::Fix::Create("nDuty", &(Controller::Trapezium::mMotorState.mLastDuty)));
	trap->Add(File::Fix::Create("step", &(Controller::Trapezium::mMotorState.mStep)));

	return trap;
}

Directory* CreatePID(){
	auto* pid = Directory::Create("pid");
	//pid->Add(File::Fix::Create("duty", &(Controller::PID::LastDuty)));

	return pid;
}

Directory* CreateBasic(){
	auto* basic = Directory::Create("basic");

	basic->Add(CreateDuty());
	basic->Add(CreateFree());
	basic->Add(CreateLock());

	return basic;
}

File::FileBase* CreateDuty(){
	return CreateExecute("duty",[](text_iterator begin,text_iterator end)->int{
		if (std::distance(begin,end)>=2){
			begin++;
			Motor::SetDuty(common::ToFix(*begin));
		}else{
			Motor::Lock();
		}
		return 0;
	});

}
File::FileBase* CreateFree(){
	return CreateExecute("free",[](text_iterator being,text_iterator end)->int{
		Motor::Free();
		return 0;
	});
}

File::FileBase* CreateLock(){
	return CreateExecute("lock",[](text_iterator being,text_iterator end)->int{
		Motor::Lock();
		return 0;
	});
}

File::FileBase* CreateSwitch(){
	return CreateExecute("switch", [](text_iterator begin, text_iterator end)->int{
		if(std::distance(begin, end) == 1){
			XPort::WriteLine("type this\nswitch 0:test 1:trapezium 2:pid");
			return 0;
		}

		begin++;
		XPort::WriteLine(Controller::SwitchControlMode((Controller::ControlMode_e)common::ToInt32((*begin))));
		return 0;
	});
}

}

} /* namespace App */
