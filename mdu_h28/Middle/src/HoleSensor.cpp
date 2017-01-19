#include <HoleSensor.hpp>
#include <INT.hpp>
#include <xport.hpp>

namespace Middle {
namespace HoleSensor{

using namespace Device;
using namespace Device::INT;
using namespace Device::Port;

static HoleSensorHandler handler;

const IntHandler INTHandler(INT_ID);

void Init(){
	//GPIOの初期化
	HoleU.Din();
	HoleV.Din();
	HoleW.Din();
	//登録する
	SetInt(HoleU, INT_ID::INT0);
	SetInt(HoleV, INT_ID::INT1);
	SetInt(HoleW, INT_ID::INT2);
	//割り込み先を登録
	SetHandler(INT_ID::INT0, INTHandler);
	SetHandler(INT_ID::INT1, INTHandler);
	SetHandler(INT_ID::INT2, INTHandler);

	handler=nullptr;
}

void SetHandler(const HoleSensorHandler& func){
	handler=func;
}

const IntHandler INTHandler(INT_ID){
	uint32_t data=0;
	data|=HoleU.Get()?0b001:0;
	data|=HoleV.Get()?0b010:0;
	data|=HoleW.Get()?0b100:0;

	//Port::LED1.Set(data & 0b001);
	//Port::LED2.Set((data & 0b010) >> 1);
	//Port::LED3.Set((data & 0b100) >> 2);

	if (handler!=nullptr){
		handler((HoleStatus)data);
	}
}



}
} /* namespace mid */
