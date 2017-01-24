#ifndef MIDDLE_HOLESENSOR_H_
#define MIDDLE_HOLESENSOR_H_

#include <functional>
#include <stdint.h>

namespace Device {
namespace HoleSensor {
void Init();

enum class HoleStatus
	:uint32_t {
		None = 0b000,//Error
	U = 0b001,
	UV = 0b011,
	VU=UV,
	V = 0b010,
	VW = 0b110,
	WV=VW,
	W = 0b100,
	WU = 0b101,
	UW=WU,
	Open=0b111//Error
};



using HoleSensorHandler=std::function<void(HoleSensor::HoleStatus)>;

void SetHandler(const HoleSensorHandler&func = nullptr);

HoleStatus GetState();

const char* const GetName(HoleStatus);
}
} /* namespace mid */

#endif /* MIDDLE_HOLESENSOR_H_ */
