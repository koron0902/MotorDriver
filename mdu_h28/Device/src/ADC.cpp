#include <ADC.hpp>
#include <chip.hpp>
#include <unit.hpp>
using namespace common;

namespace Device {

namespace ADC {

static constexpr uint32_t ADCSampleClock = 50_KHz;

//取得したデータはとりあえずここで保持する。後々、LPFでも掛けてくれ。
static uint16_t AmpA, AmpB, AmpC;
static fix32 VlotRaw, VlotRawA, VlotRawB, VlotRawC;

static const fix32 VlotGain = fix32::CreateFloat(10.0f);
static const fix32 ADCRef = fix32::CreateFloat(2.5f); //[V]
static const uint32_t ADCMax = (1 << 12) - 1;

uint16_t GetAmpA() {
	return AmpA;
}
uint16_t GetAmpB() {
	return AmpB;
}
uint16_t GetAmpC() {
	return AmpC;
}

fix32 GetVlot() {
	return VlotRaw*ADCRef*VlotGain;
}
fix32 GetVlotA() {
	return VlotRawA*ADCRef*VlotGain;
}
fix32 GetVlotB() {
	return VlotRawB*ADCRef*VlotGain;
}
fix32 GetVlotC() {
	return VlotRawC*ADCRef*VlotGain;
}

void Init() {

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
					ADC_SEQ_CTRL_MODE_EOS);
//Note 電圧検知(SV(1,2),SV_C(1,3),SV_B(1,5),SV_A(1,4)) 補足 SV_Bは本来LED0の場所である。
	Chip_ADC_SetupSequencer(LPC_ADC1,
			ADC_SEQA_IDX, //
			ADC_SEQ_CTRL_CHANSEL(
					2)|ADC_SEQ_CTRL_CHANSEL(3)|ADC_SEQ_CTRL_CHANSEL(5)|ADC_SEQ_CTRL_CHANSEL(4)| //
					ADC_SEQ_CTRL_MODE_EOS
					);

//補正を掛ける
	Chip_ADC_StartCalibration(LPC_ADC0);
	while (!Chip_ADC_IsCalibrationDone(LPC_ADC0))
		;
	Chip_ADC_StartCalibration(LPC_ADC1);
	while (!Chip_ADC_IsCalibrationDone(LPC_ADC1))
		;

//Cross Point、割り込みを作成する場合ここに挿入。

//割り込み
	Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));
	Chip_ADC_ClearFlags(LPC_ADC1, Chip_ADC_GetFlags(LPC_ADC1));

	Chip_ADC_EnableInt(LPC_ADC0, ADC_INTEN_SEQA_ENABLE);
	Chip_ADC_EnableInt(LPC_ADC1, ADC_INTEN_SEQA_ENABLE);

	NVIC_EnableIRQ(ADC0_SEQA_IRQn);
	NVIC_EnableIRQ(ADC1_SEQA_IRQn);

//実際に処理を開始する。
	Chip_ADC_EnableSequencer(LPC_ADC0, ADC_SEQA_IDX);
	Chip_ADC_EnableSequencer(LPC_ADC1, ADC_SEQA_IDX);

}

void Trigger() {
//割り込み
	Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
	Chip_ADC_StartSequencer(LPC_ADC1, ADC_SEQA_IDX);
}

extern "C" void ADC0A_IRQHandler(void) {

	/* Get pending interrupts */


//ここで結果を得る。
//Note 電流感知(SA_B(0,10),SA_A(0,9),SA_C(0,11))
	AmpA = Chip_ADC_GetDataReg(LPC_ADC0, 9);
	AmpB = Chip_ADC_GetDataReg(LPC_ADC0, 10);
	AmpC = Chip_ADC_GetDataReg(LPC_ADC0, 11);

	/* Clear any pending interrupts */
//Chip_ADC_ClearFlags(LPC_ADC0, pending);
	Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));
}

extern "C" void ADC1A_IRQHandler(void) {


//ここで結果を得る
//Note 電圧検知(SV(1,2),SV_C(1,3),SV_B(1,5),SV_A(1,4)) 補足 SV_Bは本来LED0の場所である。
	VlotRaw = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC1, 2)&0xFFFF );
	VlotRawA = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC1, 3)&0xFFFF );
	VlotRawB = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC1, 5)&0xFFFF );
	VlotRawC = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC1, 4) &0xFFFF);

	/* Clear Sequence A completion interrupt */
//Chip_ADC_ClearFlags(LPC_ADC1, pending);
	Chip_ADC_ClearFlags(LPC_ADC1, Chip_ADC_GetFlags(LPC_ADC1));
}

}

} /* namespace Device */

