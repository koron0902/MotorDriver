#include <ADC.hpp>
#include <chip.hpp>
#include <unit.hpp>
#include <Timer.hpp>
using namespace common;

namespace Device {

namespace ADC {
//回路による定数
static constexpr uint32_t ADCSampleClock = 250_KHz;
static constexpr uint32_t TriggerRate=50_KHz;
static constexpr fix32 ADCRef = fix32::CreateDouble(2.495); //[V] 基準電圧の実測値
static constexpr fix32 Resistance = fix32::CreateDouble(5); //[mΩ]シャント抵抗値
static constexpr fix32 VlotGain = fix32::CreateDouble(10.0f); //分圧回路による分圧率
//
static constexpr int32_t Correct = 1000; //補正値[k]
static constexpr uint32_t ADCMax = (1 << 12) - 1;
static fix32 AmpGain = fix32::CreateFloat(1.0f); //DRVで再設定してね。
//取得したデータはとりあえずここで保持する。これらには[0,1)が入る
static fix32 AmpRawU, AmpRawV, AmpRawW;
static fix32 VlotRaw, VlotRawU, VlotRawV, VlotRawW;

void SetAmpGain(const fix32& gain) {
	AmpGain = gain;
}

fix32 GetVlot() {
	return VlotRaw * ADCRef * VlotGain;
}

fix32 GetVlotU() {
	return VlotRawU * ADCRef * VlotGain;
}

fix32 GetVlotV() {
	return VlotRawV * ADCRef * VlotGain;
}

fix32 GetVlotW() {
	return VlotRawW * ADCRef * VlotGain;
}

fix32 GetAmpU() {
	fix32 vlot = AmpRawU * ADCRef; //実際にマイコンにかかっている電圧[V]
	fix32 gain = Resistance * AmpGain;
	return (vlot / gain) / Correct;
}
fix32 GetAmpV() {
	fix32 vlot = AmpRawV * ADCRef; //実際にマイコンにかかっている電圧[V]
	fix32 gain = Resistance * AmpGain;
	return (vlot / gain) / Correct;
}

fix32 GetAmpW() {
	fix32 vlot = AmpRawW * ADCRef; //実際にマイコンにかかっている電圧[V]
	fix32 gain = Resistance * AmpGain;
	return (vlot / gain) / Correct;
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

	//周期的に呼び出されるようにTimerに登録する。
	Timer::SetAction(1,TriggerRate,Trigger);

}

void Trigger() {
	//割り込み
	Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
	Chip_ADC_StartSequencer(LPC_ADC1, ADC_SEQA_IDX);
}

extern "C" void ADC0A_IRQHandler(void) {
//ここで結果を得る。
//Note 電流感知(SA_B(0,10),SA_A(0,9),SA_C(0,11))
	//AmpA = Chip_ADC_GetDataReg(LPC_ADC0, 9);
	AmpRawU = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC0, 9) & 0xFFF0);
	//AmpB = Chip_ADC_GetDataReg(LPC_ADC0, 10);
	AmpRawV = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC0, 10) & 0xFFF0);
	//AmpC = Chip_ADC_GetDataReg(LPC_ADC0, 11);
	AmpRawW = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC0, 11) & 0xFFF0);
	Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));

}

extern "C" void ADC1A_IRQHandler(void) {
//Note 電圧検知(SV(1,2),SV_C(1,3),SV_B(1,5),SV_A(1,4)) 補足 SV_Bは本来LED0の場所である。
	VlotRaw = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC1, 2) & 0xFFF0);
	VlotRawU = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC1, 3) & 0xFFF0);
	VlotRawV = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC1, 5) & 0xFFF0);
	VlotRawW = fix32::CreateRaw(Chip_ADC_GetDataReg(LPC_ADC1, 4) & 0xFFF0);

	Chip_ADC_ClearFlags(LPC_ADC1, Chip_ADC_GetFlags(LPC_ADC1));
}

}

} /* namespace Device */

