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
	pid->Add(CreateParam());
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
		uint32_t index;
		cmdParser.Parse(begin, end - 1);

		if(cmdParser.IsOptionNull()){
			std::string comment = "";
			comment += "switch [-c controller_type] | [-m motor_type]\n";
			comment += " -c | controller mode\n";
			comment += "   0: test, 1: trapeziom, 2: pid, 3: impulse\n";
			comment += " -m | motor type\n";
			comment += "   0: none, 1: DC, 2: BLDC(Hall-Sensor)\n";
			comment += "ex. switch -c 0 | change to test mode";

			XPort::WriteLine(comment);
			return 0;
		}

		// -cオプションが存在した場合にはコントローラの変更
		if(cmdParser.Search("-c", &index)){
			XPort::WriteLine(Controller::SwitchControlMode((Controller::ControlMode_e)common::ToInt32(cmdParser.GetOptionArg(index))));
		}

		// -mオプションが存在した場合にはモータタイプを変更
		if(cmdParser.Search("-m", &index)){
			XPort::WriteLine(Motor::SwitchMotorType((Motor::Type)common::ToInt32(cmdParser.GetOptionArg(index))));
		}

		return 0;
	});
}

File::FileBase* CreateParam(){
	return CreateExecute("param", [](text_iterator begin, text_iterator end)->auto{
		uint32_t index;
		cmdParser.Parse(begin, end - 1);
		if(cmdParser.IsOptionNull()){
			XPort::WriteLine("Option Null");
			return 0;
		}

		if(cmdParser.Search("-s", &index) && cmdParser.Search("-g", &index)){

		}else if(cmdParser.Search("-s", &index)){
			if(cmdParser.Search("-p", &index)){
				Controller::PID::SetGainP(common::ToFloat(cmdParser.GetOptionArg(index)));
			}

			if(cmdParser.Search("-i", &index)){
				Controller::PID::SetGainI(common::ToFloat(cmdParser.GetOptionArg(index)));
			}

			if(cmdParser.Search("-d", &index)){
				Controller::PID::SetGainD(common::ToFloat(cmdParser.GetOptionArg(index)));
			}

			if(cmdParser.Search("-speed", &index)){
				Controller::PID::SetTargetSpeed(common::ToFloat(cmdParser.GetOptionArg(index)));
			}

		}else if(cmdParser.Search("-g", &index)){
			if(cmdParser.Search("-p", &index)){
				XPort::WriteLine(Controller::PID::GetGainP());
			}

			if(cmdParser.Search("-i", &index)){
				XPort::WriteLine(Controller::PID::GetGainI());
			}

			if(cmdParser.Search("-d", &index)){
				XPort::WriteLine(Controller::PID::GetGainD());
			}
		}
		return 0;
	});
}

}

} /* namespace App */
