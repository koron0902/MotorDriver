/*
 * ControllerBase.cpp
 *
 *  Created on: 2016/12/11
 *      Author: Keita
 */

//#include <Trapezium.hpp>
//#include <PIDControl.hpp>
#include <Timer.hpp>
#include <motor.hpp>
//#include <Port.hpp>
#include <configuration.hpp>
#include <Control.hpp>
#include <QEI.hpp>
#include <XPort.hpp>
#include <utilize.hpp>
#include <MotorSub.hpp>
#include <ControlBase.h>
#include <chip.hpp>
#include <ControlPID.hpp>

namespace Middle {
namespace Control {
using namespace Device;

//static ControlMode Mode;
static ControlInterface *control;
static const Timer::TimerID timer_id = Timer::TimerID::Timer0;
extern uint32_t frequency;/*ControlBaseに実体がある*/
static const constexpr uint32_t FREQUENCY_DEFAULT{10_KHz };
//補助関数
//所有権をもったままfunctionをするもの
static void timer_handler(void) {
	if (control != nullptr) {
		(*control)();
	}else{
		//起動しているのはおかしいので停止
		Timer::KillHandler(timer_id);
	}
}

void Init() {
	//管理変数の初期化を行う
	Release(control);
	frequency=FREQUENCY_DEFAULT;
	//タイマーを無効化
	Timer::KillHandler(timer_id);
	//各制御モードを初期化
	PID::Init();
}

void ChangeFrequency(uint32_t value) {
	if (value > 0) {
		frequency = value;
		//実行中ならそのタイマーの値も変更する
		if (control != nullptr) {
			auto period = SystemCoreClock / frequency;
			Timer::ChangePeriod(timer_id, period);
		}
		XPort::WriteLine("Changed Control Period");
	}
}

void SwitchControlMode(ControlMode m) {
//Controlを無効化する
	if (control != nullptr) {
		if (control->GetMode() != m) {
			Timer::KillHandler(timer_id);
			Release(control);
			XPort::WriteLine("Release Handler");
		} else {
			//処理をする必要がない
			XPort::WriteLine("Keep Handler");
			return;
		}
	}
//次の状態に遷移する
	auto period = SystemCoreClock / frequency;
	switch (m) {
	case ControlMode::PID:
		control = new PID::PID();
		Timer::SetHandler(timer_handler, timer_id, period );
		XPort::WriteLine("Switch PID");
		break;
	case ControlMode::None:
	default:
		XPort::WriteLine("Switch none");
	}
}

ControlMode GetMode() {
	if (control!=nullptr){
		return control->GetMode();
	}else{
		return ControlMode::None;
	}
}

} /* namespace Controller */
} /* namespace Middle */
