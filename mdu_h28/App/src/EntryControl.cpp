#include <EntryControl.h>
#include <File.hpp>

#include <ControlPID.hpp>
#include <Control.hpp>
#include <text.hpp>
namespace App {
namespace Middle {
using namespace File;
using namespace ::Middle::Control;
using namespace common;

Directory* CreateControl() {
	Directory *ctrl = CreateDirectory("ctrl"), *pid;

	//PIDについて
	ctrl->Add(pid = CreateDirectory("pid"));
	//各種パラメータ
	//PID基礎パラメータ
	pid->Add(Float::Create("p", &PID::Kp));
	pid->Add(Float::Create("i", &PID::Ki));
	pid->Add(Float::Create("d", &PID::Kd));
	pid->Add(Float::Create("e", &PID::Ke));
	//PID拡張パラメータ
	pid->Add(Float::Create("gear", &PID::GearRate));
	pid->Add(Float::Create("radius", &PID::Radius));
	pid->Add(Float::Create("pulse", &PID::Pulse));
	//目標値
	pid->Add(Float::Create("tar", &PID::Target));

	//
	/*ctrl->Add(CreateProperty("mode", [](const std::string& inst) {
	 //Setter
	 int index=0;
	 if (IsNumberPattern(inst)) {
	 index= ToInt32(inst);
	 }

	 SwitchControlMode((ControlMode)index);

	 }, []() {
	 //Getter
	 return GetModeName();
	 }));
	 */
	return ctrl;
}

}
} /* namespace App */
