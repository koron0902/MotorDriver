
#ifndef MIDDLE_HOLESENSOR_H_
#define MIDDLE_HOLESENSOR_H_

#include <functional>
#include <stdint.h>

namespace Middle {
namespace HoleSensor{
void Init();

enum class HoleStatus:uint32_t{
	None=0b000,
	U=0b001,
	UV=0b011,
	V=0b010,
	VW=0b110,
	W=0b100,
	WU=0b101
};

using HoleSensorHandler=std::function<void(HoleSensor::HoleStatus)>;

void SetHandler(const HoleSensorHandler&func=nullptr);



}
} /* namespace mid */

#endif /* MIDDLE_HOLESENSOR_H_ */
