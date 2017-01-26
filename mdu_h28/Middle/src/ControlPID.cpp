/*
 * PID.cpp
 *
 *  Created on: 2017/01/25
 *      Author: hamus
 */

#include <unit.hpp>
#include <matrix.hpp>
#include <ADC.hpp>
#include <QEI.hpp>
#include <configuration.hpp>
#include <ControlPID.hpp>
#include <qmath.hpp>
#include <Motor.hpp>
#include <Timer.hpp>
#include <type.hpp>
namespace Middle {
namespace Control {
namespace PID{
using namespace common;
using namespace std;
//各種初期値

static const constexpr real_t GEAR_RATE_DEFAULT{1 };
static const constexpr real_t RADIUS_DEFAULT{1 };
static const constexpr real_t ENCODER_RESOLUTION_DEFAULT{400 };
static const constexpr real_t K_P_DEFAULT{0.02819 };
static const constexpr real_t K_I_DEFAULT{0.00242 };
static const constexpr real_t K_D_DEFAULT{0 };
static const constexpr real_t K_E_DEFAULT{1.0 / 6.23333 };

static const constexpr uint8_t TimerChannel=1;//使用するタイマーちゃんねる
//制御値
float GearRate;	// モータ側 /　タイヤ側
float Radius;		// タイヤ半径
float Pulse
;	// エンコーダの分解能 [ppr]
float Kp;	// 比例ゲイン
float Ki;	// 積分ゲイン
float Kd;	// 微分ゲイン
float Ke;	// 逆起電力ゲイン
real_t Target;	//目標値

static fix32 LastDuty;	//最終出力

void Init() {
	//ひたすらコピーしていく
	GearRate = GEAR_RATE_DEFAULT;
	Radius = RADIUS_DEFAULT;
	Pulse
 = ENCODER_RESOLUTION_DEFAULT;
	Kp = K_P_DEFAULT;
	Ki = K_I_DEFAULT;
	Kd = K_D_DEFAULT;
	Ke = K_E_DEFAULT;

	LastDuty = 0;
}

PID::PID() :ControlInterface(ControlMode::PID){
	//ハンドラーを設定
	next.Reset();
	last.Reset();

}

PID::~PID() {

}

void PID::operator()() {
	static const float gains[] = {Kp, Ki, Kd, Ke };
	static const Matrix<float, 1, 4> GainMatrix(gains);
	//for(int i = 0;i < 4;i++)
	//GainMatrix(0, i) = gains[i];
	static Matrix<float, 4, 1> in_vector;

	static const auto BatteryVoltage = (float) Device::ADC::GetVolt() * 100;
	static const auto Volt2Duty = 4096 * 6.25f / (BatteryVoltage / 16);
	static const auto Pulse2RPS =GetFrequency() / Pulse
;

	next.RealSpeed = Device::QEI::GetVelcoity() * Pulse2RPS;

	next.TargetSpeed = next.RealSpeed;
	static const auto error = next.TargetSpeed - next.RealSpeed;
	static auto integ = last.Integration + error;
	static constexpr auto min = -10.0f;		//<< fix32::shift;
	static constexpr auto max = 10.0f;		// << fix32::shift;
	integ = (integ < min ? min : (integ > max ? max : integ));

	in_vector(0, 0) = error;
	in_vector(1, 0) = integ;
	in_vector(2, 0) = error - last.Error;
	in_vector(3, 0) = next.RealSpeed;

	auto output = GainMatrix * in_vector;

	output(0, 0) *= Volt2Duty;
	auto duty = output(0, 0);
	duty /= 4096;
	duty = (duty < -1 ? -1 : (duty > 1 ? 1 : duty));

	next.Integration = integ;
	next.Error = error;
	next.Duty = duty;

	Middle::Motor::SetDuty(regions::one.Fit(fix32::CreateFloat(duty)));
	last = next;
	LastDuty = last.Duty;
}
}
}
} /* namespace mid */
