/*
 * DRV.h
 *
 *  Created on: 2016/11/13
 *      Author: Keita
 */

#ifndef DEVICE_INC_DRV_H_
#define DEVICE_INC_DRV_H_

#include "type.hpp"
#include "DRVRegister.hpp"

namespace Middle{
	namespace DRV{

		void Init();	// 言わずもがな初期化関数

		bool SetConfiguration(const DRVRegName reg, const uint16_t data);	// レジスタに設定を書き込むための関数
		bool SetGain(const uint16_t ch, const GAIN_T gain);		// 直接ゲインを書き換える
		bool SetPWMMode(const PWM_USES_T mode);	// 直接PWMモードを書き換える

		uint16_t GetRegValue(const DRVRegName reg);	// updateにより読み出されているレジスタ値をこれで取得する

		void Update(); //　これは定期的に呼ばれてDRVのレジスタ値を読み出すやつ

		// ここから下はFaultに関するやつ
		bool IsFault();
		bool WarnHighTempereture();
		bool WarnPVDDLow();
		bool WarnPVDDHigh();
		bool WarnCHPLow();
		bool IsOverCurrent();
		bool IsOverCurrentHA();
		bool IsOverCurrentLA();
		bool IsOverCurrentHB();
		bool IsOverCurrentLB();
		bool IsOverCurrentHC();
		bool IsOverCurrentLC();
		bool IsPVDDLow();
		bool IsWatchDog();
		bool IsOverTemperature();
		bool IsVREGLow();
		bool IsAVDDLow();
		bool IsLSSupply();
		bool IsCHPLow();
		bool IsCHPHigh();
		bool IsCHPHighABS();
		bool IsVGSHA();
		bool IsVGSLA();
		bool IsVGSHB();
		bool IsVGSLB();
		bool IsVGSHC();
		bool IsVGSLC();
	}
}

#endif /* DEVICE_INC_DRV_H_ */
