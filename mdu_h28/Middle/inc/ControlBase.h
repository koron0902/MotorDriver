#pragma once
#ifndef MIDDLE_INC_CONTROLBASE_H_
#define MIDDLE_INC_CONTROLBASE_H_

#include <stdint.h>

/* このファイルはControlの共通部を記述するために用いる。
 */

namespace Middle {
namespace Control {
//Errorを必ず最後にすること
enum class ControlMode
	:uint32_t {
		None, /*Trapezium ,*/PID, Error
};

uint32_t GetFrequency();
/*SetFrequencyは意図的に定義していない*/

const char * const GetName(ControlMode);

class ControlInterface {
	ControlMode mode;
public:
	ControlInterface(ControlMode);
	ControlInterface(const ControlInterface&) = default;
	virtual ~ControlInterface();
	virtual void operator ()() {
	}
	ControlMode GetMode() const {
		return mode;
	}
};

}
}
#endif /* MIDDLE_INC_CONTROLBASE_H_ */
