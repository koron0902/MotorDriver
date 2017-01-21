#include <HoleSensor.hpp>
#include <INT.hpp>
#include <xport.hpp>
#include <configuration.hpp>
namespace Middle {
namespace HoleSensor {

using namespace Device;
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
	auto int_handler=[](INT_ID id){
		if (handler!=nullptr){
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

HoleStatus GetState() {
	uint32_t data = 0;
	data |= HoleU.Get() ? 0b001 : 0;
	data |= HoleV.Get() ? 0b010 : 0;
	data |= HoleW.Get() ? 0b100 : 0;
	return (HoleStatus) data;
}



}
} /* namespace mid */
