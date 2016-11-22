#include <chip.h>
#include <Timer.hpp>
#include <array>

using namespace std;

namespace Device{
namespace Timer{

static array<function<void(void)>, MRT_CHANNELS_NUM> lstAction;

void Init(){
	//Supply clock
	Chip_MRT_Init();
	Chip_RIT_Init(LPC_RITIMER);
	//Disable all timer
	for (int ch=0;ch<MRT_CHANNELS_NUM;ch++){
		Chip_MRT_SetDisabled(Chip_MRT_GetRegPtr(ch));
		lstAction[ch]=nullptr;
	}

	ResetSystemTimer();
	//Allow interrupt by timer
	NVIC_EnableIRQ(MRT_IRQn);
}

void SetAction(uint8_t ch,uint32_t hz,const function<void(void)>& action){
	//hzに0を入れる馬鹿はいないと信じている。
	auto* p= Chip_MRT_GetRegPtr(ch);
	lstAction[ch]=action;
	if (action!=nullptr){
		Chip_MRT_SetDisabled(p);//一回動作を止める。
		Chip_MRT_SetInterval(p,SystemCoreClock/hz|MRT_INTVAL_LOAD);//強制書き換え
		Chip_MRT_SetMode(p,MRT_MODE_REPEAT);
		Chip_MRT_SetEnabled(p);
	}else{
		Chip_MRT_SetDisabled(p);
	}
}

void ResetSystemTimer(){
	Chip_RIT_Disable(LPC_RITIMER);
	LPC_RITIMER->COUNTER=0;
	LPC_RITIMER->COUNTER_H=0;
	Chip_RIT_Enable(LPC_RITIMER);
}

clock_t GetSystemTime(){
	auto *p=LPC_RITIMER;
	return (clock_t)((uint64_t)p->COUNTER_H<<32)+(uint64_t)p->COUNTER;
}

void Delay(clock_t clk){
	ResetSystemTimer();
	while (GetSystemTime()<clk);
}

extern "C"
void MRT_IRQHandler(void)
{
	uint32_t int_pend;
	/* Get and clear interrupt pending status for all timers */
	int_pend = Chip_MRT_GetIntPending();
	Chip_MRT_ClearIntPending(int_pend);//次の割り込み、お待ちしてます。
	
	for (int ch=0;ch<MRT_CHANNELS_NUM;ch++){
		if (int_pend&MRTn_INTFLAG(ch)){
			auto func=lstAction[ch];
			if (func!=nullptr)func();
		}
	}
}


}
}
