/*
 * ControllerBase.h
 *
 *  Created on: 2016/12/11
 *      Author: Keita
 */

#ifndef MIDDLE_INC_CONTROLLERBASE_HPP_
#define MIDDLE_INC_CONTROLLERBASE_HPP_

#include <fix.hpp>
#include <unit.hpp>
#include <type.hpp>

namespace Middle {
namespace Controller {
using namespace common;

enum class ControlMode
	:uint32_t {
		None , Trapezium , PID ,Error
};

void SwitchControlMode(ControlMode _mode);
ControlMode GetMode();
const char *const GetName(ControlMode);
static inline const char* const GetModeName(){
	return GetName(GetMode());
}


class ControllerBase {
private:

protected:
	fix32 mFreq;
	static constexpr fix32 FREQ_DEFAULT = (10_KHz << fix32::shift);
	static constexpr fix32 STEP_DEFAULT = (0.002 * fix32::gain);

	std::function<void(void)> CallProc = nullptr;

public:
	static constexpr uint32_t mControllerTaskPriority = 1;
	ControllerBase();
	ControllerBase(const ControllerBase&) = default;
	virtual ~ControllerBase();

	uint32_t GetFreq() {
		return (int32_t) mFreq;
	}

	void operator ()(void) {
		if (CallProc != nullptr) CallProc();
	}

};

} /* namespace Controller */
} /* namespace Middle */

#endif /* MIDDLE_INC_CONTROLLERBASE_HPP_ */
