#include <ADC.hpp>
#include <chip.h>
#include <unit.hpp>
using namespace common;

namespace Device {

namespace ADC {

static constexpr uint32_t ADCSampleClock = 50_KHz;

//取得したデータはとりあえずここで保持する。後々、LPFでも掛けてくれ。
static uint16_t AmpA,AmpB,AmpC;
static uint16_t Vlot,VlotA,VlotB,VlotC;

uint16_t GetAmpA(){return AmpA;}
uint16_t GetAmpB(){return AmpB;}
uint16_t GetAmpC(){return AmpC;}

uint16_t GetVlot(){return Vlot;}
uint16_t GetVlotA(){return VlotA;}
uint16_t GetVlotB(){return VlotB;}
uint16_t GetVlotC(){return VlotC;}

void Init() {
#if 0

	//supply clock
	Chip_ADC_Init(LPC_ADC0, 0);
	Chip_ADC_Init(LPC_ADC1, 0);

	//trigger by clock,auto mode
	Chip_ADC_SetClockRate(LPC_ADC0, SystemCoreClock / ADCSampleClock);
	Chip_ADC_SetClockRate(LPC_ADC1, SystemCoreClock / ADCSampleClock);

	//SetUp
	//Note 電流感知(SA_B(0,10),SA_A(0,9),SA_C(0,11))
	Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX, //
			ADC_SEQ_CTRL_CHANSEL(
					10) | ADC_SEQ_CTRL_CHANSEL(9) | ADC_SEQ_CTRL_CHANSEL(11) |	//
					ADC_SEQ_CTRL_BURST);
	//Note 電圧検知(SV(1,2),SV_C(1,3),SV_B(1,5),SV_A(1,4)) 補足 SV_Bは本来LED0の場所である。
	Chip_ADC_SetupSequencer(LPC_ADC1,
			ADC_SEQA_IDX, //
			ADC_SEQ_CTRL_CHANSEL(
					2)|ADC_SEQ_CTRL_CHANSEL(3)|ADC_SEQ_CTRL_CHANSEL(5)|ADC_SEQ_CTRL_CHANSEL(4)| //
					ADC_SEQ_CTRL_BURST
					);



	//補正を掛ける
	Chip_ADC_StartCalibration(LPC_ADC0);
	Chip_ADC_StartCalibration(LPC_ADC1);
	while (!Chip_ADC_IsCalibrationDone(LPC_ADC0));
	while (!Chip_ADC_IsCalibrationDone(LPC_ADC1));

	//Cross Point、割り込みを作成する場合ここに挿入。

	//割り込み
	Chip_ADC_ClearFlags(LPC_ADC0,Chip_ADC_GetFlags(LPC_ADC0));
	Chip_ADC_ClearFlags(LPC_ADC1,Chip_ADC_GetFlags(LPC_ADC1));


	Chip_ADC_EnableInt(LPC_ADC0,ADC_INTEN_SEQA_ENABLE);
	Chip_ADC_EnableInt(LPC_ADC1,ADC_INTEN_SEQA_ENABLE);

	NVIC_EnableIRQ(ADC0_SEQA_IRQn);
	NVIC_EnableIRQ(ADC1_SEQA_IRQn);


	//実際に処理を開始する。
	Chip_ADC_EnableSequencer(LPC_ADC0,ADC_SEQA_IDX);
	Chip_ADC_EnableSequencer(LPC_ADC1,ADC_SEQA_IDX);
#endif
}

extern "C"
void ADC0A_IRQHandler(void)
{
	uint32_t pending;

	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC0);

	//ここで結果を得る。
	//Note 電流感知(SA_B(0,10),SA_A(0,9),SA_C(0,11))
	AmpA=Chip_ADC_GetDataReg(LPC_ADC0,9);
	AmpB=Chip_ADC_GetDataReg(LPC_ADC0,10);
	AmpC=Chip_ADC_GetDataReg(LPC_ADC0,11);

	/* Clear any pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC0, pending);
}

extern "C"
void ADC1A_IRQHandler(void)
{
	uint32_t pending;

	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC1);

	//ここで結果を得る
	//Note 電圧検知(SV(1,2),SV_C(1,3),SV_B(1,5),SV_A(1,4)) 補足 SV_Bは本来LED0の場所である。
	Vlot=Chip_ADC_GetDataReg(LPC_ADC1,2);
	VlotA=Chip_ADC_GetDataReg(LPC_ADC1,3);
	VlotB=Chip_ADC_GetDataReg(LPC_ADC1,5);
	VlotC=Chip_ADC_GetDataReg(LPC_ADC1,4);

	/* Clear Sequence A completion interrupt */
	Chip_ADC_ClearFlags(LPC_ADC1, pending);
}

}

} /* namespace Device */


