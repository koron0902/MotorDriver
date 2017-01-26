/*
 * ControllerBase.h
 *
 *  Created on: 2016/12/11
 *      Author: Keita
 */

#ifndef MIDDLE_INC_CONTROL_HPP_
#define MIDDLE_INC_CONTROL_HPP_

#include <fix.hpp>
#include <unit.hpp>
#include <type.hpp>
#include <ControlBase.h>

namespace Middle {
namespace Control {
using namespace common;

void Init();




uint32_t GetFrequency();
//これは動的に値を変更する。(意図的にSetterにしていない)
void ChangeFrequency(uint32_t);

void SwitchControlMode(ControlMode _mode);

ControlMode GetMode();
static inline const char* const GetModeName() {
	return GetName(GetMode());
}

} /* namespace Controller */
} /* namespace Middle */

#endif /* MIDDLE_INC_CONTROL_HPP_ */
