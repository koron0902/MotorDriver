#include <Mid.hpp>
#include <Motor.hpp>
#include <text.hpp>
#include <DRV.hpp>
#include <Trapezium.hpp>
#include <PIDControl.hpp>
#include <ControllerBase.hpp>
#include <type.hpp>
#include<File.hpp>
#include <XPort.hpp>

using namespace Middle;
using namespace App::File;
using namespace common;

namespace App {
namespace Mid {
Directory* Create() {
	auto *mid = Directory::Create("mid");
	mid->Add(CreateBasic());
	mid->Add(CreateDRV());
	mid->Add(CreateTrap());
	mid->Add(CreatePID());
	mid->Add(CreateSwitch());
	return mid;
}

Directory* CreateDRV() {
	auto* drv = Directory::Create("drv");
	//drv->Add(File::CreateProperty("gain",*(Middle::DRV::GetGain),*(Middle::DRV::SetGain)));
	return drv;
}

Directory* CreateTrap() {
	auto* trap = Directory::Create("trap");
	trap->Add(
			File::Fix::Create("tDuty",
					&(Controller::Trapezium::mMotorState.mTargetDuty)));
	trap->Add(
			File::Fix::Create("nDuty",
					&(Controller::Trapezium::mMotorState.mLastDuty)));
	trap->Add(
			File::Fix::Create("step",
					&(Controller::Trapezium::mMotorState.mStep)));

	return trap;
}

Directory* CreatePID() {
	auto* pid = Directory::Create("pid");
	//pid->Add(File::Fix::Create("duty", &(Controller::PID::LastDuty)));

	return pid;
}

Directory* CreateBasic() {
	auto* basic = Directory::Create("basic");

	basic->Add(CreateDuty());
	basic->Add(CreateFree());
	basic->Add(CreateLock());

	return basic;
}

File::FileBase* CreateDuty() {
	return CreateExecute("duty",
			[](text_iterator begin,text_iterator end)->int {
				if (std::distance(begin,end)>=2) {
					begin++;
					Motor::SetDuty(common::ToFix(*begin));
				} else {
					Motor::Lock();
				}
				return 0;
			});

}

File::FileBase* CreateFree() {
	return CreateExecute("free",
			[](text_iterator being,text_iterator end)->int {
				Motor::Free();
				return 0;
			});
}

File::FileBase* CreateLock() {
	return CreateExecute("lock",
			[](text_iterator being,text_iterator end)->int {
				Motor::Lock();
				return 0;
			});
}

File::FileBase* CreateSwitch() {
	return CreateExecute("switch",
			[](text_iterator begin, text_iterator end)->int {

				if(std::distance(begin, end) <= 2) {
					XPort::WriteLine("switch [-c controller_type] | [-m motor_type]\n");

					//モーターの種類に関する出力
					{
						using namespace Motor;
						const int begin =(int)MotorType::None;
						const int end=(int)MotorType::Error;
						bool flag=false;

						XPort::WriteLine("#MotorType");
						for (int it=begin;it<end;it++) {
							string buf;
							if (flag) {
								buf="|";
							} else {
								flag=true;
							}
							buf="("+ToStr(it)+"):"+GetName((MotorType)it);
							XPort::WriteLine(buf);
						}
					}
					//モーターの制御方法に関する出力
					{
						using namespace Controller;
						const int begin =(int)ControlMode::None;
						const int end=(int)ControlMode::Error;
						bool flag=false;
						XPort::WriteLine("#ControlMode");
						for (int it=begin;it<end;it++) {
							string buf;
							if (flag) {
								buf="|";
							} else {
								flag=true;
							}
							buf="("+ToStr(it)+"):"+GetName((ControlMode)it);
							XPort::WriteLine(buf);
						}
					}

					XPort::WriteLine( "ex. switch -c 0 | change to test mode");
					XPort::Flush();
					return 0;
				}

				auto it=begin+1;
				while (distance(it,end)>=2){
					if (IsOptionPattern(*it)) {
						//パラメータを取得する
						auto next=it+1;
						int value;
						if (IsNumberPattern(*next)) {
							value= common::ToInt32(*next);
						}else{
							value=0;
						}
						if((*it) == "-c") {
							Controller::SwitchControlMode((Controller::ControlMode)value);
						} else if((*it) == "-m") {
							Motor::SwitchMotorType((Motor::MotorType)value);
						}else{
							XPort::WriteLine("Error:Not Support");
							return -1;
						}
						it+=2;
					}else{
						//無視して次へ
						it++;
					}

				}

				return 0;
			});
}

}

} /* namespace App */
