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
#include <functional>

namespace Middle {
	namespace Controller {
		using namespace common;
		class ControllerBase {
		private:

		protected:
			fix32 mFreq;
			static constexpr fix32 FREQ_DEFAULT = (1_KHz << fix32::shift);
			static constexpr fix32 STEP_DEFAULT = (0.002 * fix32::gain);

			std::function<void(void)> CallProc = nullptr;

		public:
			static constexpr uint32_t mControllerTaskPriority = 1;
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

		enum class ControlMode_e:uint32_t{
			ModeTest = 0,
			ModeTrapezium = 1,
			ModePID = 2
		};
		extern ControlMode_e mMode;
		std::string SwitchControlMode(ControlMode_e _mode);

	} /* namespace Controller */
} /* namespace Middle */

#endif /* MIDDLE_INC_CONTROLLERBASE_HPP_ */
