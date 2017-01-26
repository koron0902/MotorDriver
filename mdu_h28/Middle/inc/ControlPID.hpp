/*
 * PID.hpp
 *
 *  Created on: 2017/01/25
 *      Author: hamus
 */

#ifndef MIDDLE_INC_CONTROLPID_HPP_
#define MIDDLE_INC_CONTROLPID_HPP_

#include <ControlBase.h>
#include <fix.hpp>
namespace Middle {
namespace Control {
namespace PID{
using namespace common;
//あとでプロパティ等を作成すること。
typedef float real_t;//TODO あとでfix32に変更すること

extern real_t GearRate;	// モータ側 /　タイヤ側
extern real_t Radius;		// タイヤ半径
extern real_t Pulse;	// エンコーダの分解能 [ppr]
extern real_t Kp;	// 比例ゲイン
extern real_t Ki;	// 積分ゲイン
extern real_t Kd;	// 微分ゲイン
extern real_t Ke;	// 逆起電力ゲイン

extern real_t Target;	//目標値

void Init();

struct MotorState {
	real_t Error{0};		// 偏差
	real_t Integration{0};	// 積分値
	real_t TargetSpeed{0};	// [rps]
	real_t RealSpeed{0};	// [rps]
	real_t Duty{0};
	void Reset(){
		Error=Integration=TargetSpeed=RealSpeed=Duty=0;
	}

	MotorState()=default;
	MotorState(const MotorState&)=default;
	~MotorState()=default;
};

class PID :public ControlInterface{
	//モーターの状態
	MotorState next, last;

public:
	PID();
	PID(const PID&)=default;
	virtual ~PID();
	virtual void operator ()();
	void Proc(){operator ()();}//別名
};
}
}
} /* namespace mid */

#endif /* MIDDLE_INC_CONTROLPID_HPP_ */
