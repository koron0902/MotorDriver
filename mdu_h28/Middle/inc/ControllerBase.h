/*
 * ControllerBase.h
 *
 *  Created on: 2016/12/11
 *      Author: Keita
 */

#ifndef MIDDLE_INC_CONTROLLERBASE_H_
#define MIDDLE_INC_CONTROLLERBASE_H_

#include <fix.hpp>
#include <unit.hpp>
#include <functional>

namespace Middle {
	namespace Controller {
		using namespace common;
		class ControllerBase {
		private:

		protected:
			fix32 mFreq;
			static constexpr fix32 FREQ_DEFAULT = (8_KHz << fix32::shift);
			static constexpr fix32 STEP_DEFAULT = (0.002 * fix32::gain);

			std::function<void(void)> CallProc = nullptr;

		public:
			ControllerBase();
			ControllerBase(const ControllerBase&) = default;
			virtual ~ControllerBase();

			uint32_t GetFreq(){
				return (int32_t)mFreq;
			}

			void operator ()(void){
				if(CallProc)
					CallProc();
			}

		};

	} /* namespace Controller */
} /* namespace Middle */

#endif /* MIDDLE_INC_CONTROLLERBASE_H_ */
