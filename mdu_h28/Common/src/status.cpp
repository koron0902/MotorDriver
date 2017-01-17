#include "status.hpp"

namespace common{

constexpr MotorPosition Next(MotorPosition s) {
	switch (s) {
	case MotorPosition::U:
		return MotorPosition::UV;
	case MotorPosition::UV:
		return MotorPosition::V;
	case MotorPosition::V:
		return MotorPosition::VW;
	case MotorPosition::VW:
		return MotorPosition::W;
	case MotorPosition::W:
		return MotorPosition::WU;
	case MotorPosition::WU:
		return MotorPosition::U;
	case MotorPosition::None:
	default:
		return MotorPosition::None;
	}
}

constexpr MotorPosition Previous(MotorPosition s) {
	switch (s) {
	case MotorPosition::U:
		return MotorPosition::WU;
	case MotorPosition::UV:
		return MotorPosition::U;
	case MotorPosition::V:
		return MotorPosition::UV;
	case MotorPosition::VW:
		return MotorPosition::V;
	case MotorPosition::W:
		return MotorPosition::VW;
	case MotorPosition::WU:
		return MotorPosition::W;
	case MotorPosition::None:
	default:
		return MotorPosition::None;
	}
}

}
