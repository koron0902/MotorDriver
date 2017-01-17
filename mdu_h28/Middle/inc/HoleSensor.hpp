
#ifndef MIDDLE_HOLESENSOR_H_
#define MIDDLE_HOLESENSOR_H_

#include <functional>
#include <stdint.h>
#include <status.hpp>

namespace Middle {
namespace HoleSensor{
void Init();


using HoleSensorHandler=std::function<void(common::MotorPosition)>;

void SetHandler(const HoleSensorHandler&func=nullptr);





}
} /* namespace mid */

#endif /* MIDDLE_HOLESENSOR_H_ */
