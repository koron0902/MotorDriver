#include <HoleSensor.hpp>
#include <INT.hpp>
#include <xport.hpp>
#include <configuration.hpp>
#include <bits.hpp>
namespace Device {
namespace HoleSensor {

using namespace Device::INT;
using namespace Device::Port;

static HoleSensorHandler handler;

void Init() {
	//GPIOの初期化
	HoleU.Din();
	HoleV.Din();
	HoleW.Din();
	//登録する
	SetInt(HoleU, INT_ID::INT0);
	SetInt(HoleV, INT_ID::INT1);
	SetInt(HoleW, INT_ID::INT2);
	//割り込み先を登録
	auto int_handler = [](INT_ID id) {
		if (handler!=nullptr) {
			handler(GetState());
		}
	};
	SetHandler(INT_ID::INT0, int_handler, PriorityHoleSensor);
	SetHandler(INT_ID::INT1, int_handler, PriorityHoleSensor);
	SetHandler(INT_ID::INT2, int_handler, PriorityHoleSensor);

	handler = nullptr;
}

void SetHandler(const HoleSensorHandler& func) {
	handler = func;
}

static inline HoleStatus operator |(HoleStatus a, HoleStatus b) {
	return (HoleStatus) ((uint32_t) a | (uint32_t) b);
}

HoleStatus GetState() {
	return (HoleU.Get() ? HoleStatus::U : HoleStatus::None) | //
			(HoleV.Get() ? HoleStatus::V : HoleStatus::None) | 	 //
			(HoleW.Get() ? HoleStatus::W : HoleStatus::None);	//
}

const char* const GetName(HoleStatus s) {
	switch (s) {
	case HoleStatus::None:
		return "None";
	case HoleStatus::U:
		return "U";
	case HoleStatus::UV:
		return "UV";
	case HoleStatus::V:
		return "V";
	case HoleStatus::VW:
		return "VW";
	case HoleStatus::W:
		return "W";
	case HoleStatus::WU:
		return "WU";
	case HoleStatus::Open:
		return "Open";
	default:
		return "Error";
	}
}
}
} /* namespace mid */
