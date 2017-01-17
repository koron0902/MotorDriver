/*
 * DRV.cpp
 *
 *  Created on: 2016/11/13
 *      Author: Keita
 */

#include "DRV.hpp"
#include "SPI.hpp"
#include <string>
#include <cstdlib>
#include <text.hpp>

#define WRITE		(0 << 15)
#define READ		(1 << 15)
#define REGISTER(x)	((uint16_t)x << 11)

namespace Middle {
	namespace DRV {

		struct {
			WARNING_WATCHDOG_T mWarningWatchdog;
			OV_VDS_FAULT_T mOVVDSFault;
			IC_FAULT_T mICFault;
			VGS_FAULT_T mVGSFault;
			HS_GATE_DRIVER_CTRL_T mHSDriver;
			LS_GATE_DRIVER_CTRL_T mLSDriver;
			GATE_DRIVE_CTRL_T mGateDriver;
			IC_OPERATION_T mICOparation;
			SHUNT_AMPLIFIER_CTRL_T mShuntAmp;
			VOLTAGE_REGURATOR_CTRL_T mVolRegurator;
			VDS_SENSE_CTRL_T mVDSSense;
		} DRVRegisters;

		void Init() {
			// Configuration for high side gate driver
			HS_GATE_DRIVER_CTRL_T hsGateDriverCtrl;
			hsGateDriverCtrl.IDRIVEP_HS = PEAK_SOURCE_MAX;
			hsGateDriverCtrl.IDRIVEN_HS = PEAK_SINK_MAX;
			SetConfiguration(DRVRegName::HS_GATE_DRIVER_CTRL, hsGateDriverCtrl);

			// Configuration for low side gate driver
			LS_GATE_DRIVER_CTRL_T lsGateDriverCtrl;
			lsGateDriverCtrl.IDRIVEP_LS = PEAK_SOURCE_MAX;
			lsGateDriverCtrl.IDRIVEN_LS = PEAK_SINK_MAX;
			SetConfiguration(DRVRegName::LS_GATE_DRIVER_CTRL, lsGateDriverCtrl);

			// Configuration for gate drive
			GATE_DRIVE_CTRL_T gateDriveCtrl;
			gateDriveCtrl.TVDS = DEGLITCH_TIME_7us;
			gateDriveCtrl.DEAD_TIME = DEAD_TIME_88ns;
			gateDriveCtrl.PWM_MODE = PWM_USES_1;
			SetConfiguration(DRVRegName::GATE_DRIVE_CTRL, 0b01100100111);

			IC_OPERATION_T icOperation;
			icOperation.SET_VCPH_UV = CHP_UV_THRESHOLD_4v6;
			icOperation.WD_EN = WATCH_DOG_DISABLE;
			SetConfiguration(DRVRegName::IC_OPERATION, icOperation);

			SHUNT_AMPLIFIER_CTRL_T shuntAmplifierCtrl;
			shuntAmplifierCtrl.GAIN_CS1 = GAIN_20;
			shuntAmplifierCtrl.GAIN_CS2 = GAIN_20;
			shuntAmplifierCtrl.GAIN_CS3 = GAIN_20;
			SetConfiguration(DRVRegName::SHUNT_AMPLIFIER_CTRL, shuntAmplifierCtrl);

			VOLTAGE_REGURATOR_CTRL_T voltageReguratorCtrl;
			voltageReguratorCtrl.VREF_SCALE = VREF_SCALE_2;
			voltageReguratorCtrl.SLEEP_DLY = SLEEP_DELAY_50us;
			voltageReguratorCtrl.VREG_UV_LEVEL = VREG_TO_70;
			SetConfiguration(DRVRegName::VOLTAGE_REGURATOR_CTRL, voltageReguratorCtrl);

			VDS_SENSE_CTRL_T vdsSenseCtrl;
			vdsSenseCtrl.VDS_LEVEL = THRESHOLD_2V131;
			SetConfiguration(DRVRegName::VDS_SENSE_CTRL, vdsSenseCtrl);
		}

		bool SetConfiguration(const DRVRegName reg, const uint16_t data) {
			uint16_t buf;
			uint16_t packet = WRITE | REGISTER(reg) | data;
			return Device::SPI::ReadWrite(&packet, &buf);
		}

		const std::string SetGain(const std::string& str){
			int gain = std::atoi(str.c_str());
			bool status = false;

			if(gain == 10){
				for(int ch = 1;ch < 4;ch++){
					status = status | SetGainCh(ch, GAIN_T::GAIN_10);
				}
			}else if(gain == 20){
				for(int ch = 1;ch < 4;ch++){
					status = status | SetGainCh(ch, GAIN_T::GAIN_20);
				}
			}else if(gain == 40){
				for(int ch = 1;ch < 4;ch++){
					status = status | SetGainCh(ch, GAIN_T::GAIN_40);
				}
			}else if(gain == 80){
				for(int ch = 1;ch < 4;ch++){
					status = status | SetGainCh(ch, GAIN_T::GAIN_80);
				}
			}else{
				return "Not supported gain. Gain change failed";
			}

			return status == true ? "" : "Gain change failed";
		}

		const std::string GetGain(){
			std::string buf;
			buf.reserve(48);

			auto pow2 = [](uint32_t x){
				uint32_t var = 1;
				for(uint32_t i = 0;i < x;i++){
					var *= 2;
				}
				return var;
			};
			SHUNT_AMPLIFIER_CTRL_T var;
			var = GetRegValue(DRVRegName::SHUNT_AMPLIFIER_CTRL);
			buf.append("Phase U:" + common::ToStr(10 * pow2(var.GAIN_CS1)) + "\r\n");
			buf.append("Phase V:" + common::ToStr(10 * pow2(var.GAIN_CS2)) + "\r\n");
			buf.append("Phase W:" + common::ToStr(10 * pow2(var.GAIN_CS3)) + "\r\n");

			return buf;
		}

		bool SetGainCh(const uint16_t ch, const GAIN_T gain){
			SHUNT_AMPLIFIER_CTRL_T var;
			var = GetRegValue(DRVRegName::SHUNT_AMPLIFIER_CTRL);
			if(ch == 1)
				var.GAIN_CS1 = gain;
			if(ch == 2)
				var.GAIN_CS2 = gain;
			if(ch == 3)
				var.GAIN_CS3 = gain;
			if(SetConfiguration(DRVRegName::SHUNT_AMPLIFIER_CTRL, var)){
				DRVRegisters.mShuntAmp = var;
				return true;
			}
			return false;
		}

		bool SetPWMMode(const PWM_USES_T mode){
			GATE_DRIVE_CTRL_T var;
			var = GetRegValue(DRVRegName::GATE_DRIVE_CTRL);
			var.PWM_MODE = mode;
			return SetConfiguration(DRVRegName::GATE_DRIVE_CTRL, var);
		}

		uint16_t GetRegValue(const DRVRegName reg) {

			switch (reg) {
				case DRVRegName::WARNING_WATCHDOG:
					return DRVRegisters.mWarningWatchdog;

				case DRVRegName::OV_VDS_FAULT:
					return DRVRegisters.mOVVDSFault;

				case DRVRegName::IC_FAULT:
					return DRVRegisters.mICFault;

				case DRVRegName::VGS_FAULT:
					return DRVRegisters.mVGSFault;

				case DRVRegName::HS_GATE_DRIVER_CTRL:
					return DRVRegisters.mHSDriver;

				case DRVRegName::LS_GATE_DRIVER_CTRL:
					return DRVRegisters.mLSDriver;

				case DRVRegName::GATE_DRIVE_CTRL:
					return DRVRegisters.mGateDriver;

				case DRVRegName::IC_OPERATION:
					return DRVRegisters.mICOparation;

				case DRVRegName::SHUNT_AMPLIFIER_CTRL:
					return DRVRegisters.mShuntAmp;

				case DRVRegName::VOLTAGE_REGURATOR_CTRL:
					return DRVRegisters.mVolRegurator;

				case DRVRegName::VDS_SENSE_CTRL:
					return DRVRegisters.mVDSSense;

				default:
					return -1;
			}
		}

		static inline void Read(const DRVRegName reg, uint16_t* buf) {
			uint16_t packet = READ | REGISTER(reg);
			Device::SPI::ReadWrite(&packet, buf);
		}

		void Update() {
			static uint16_t buf;

			Read(DRVRegName::WARNING_WATCHDOG, &buf);
			DRVRegisters.mWarningWatchdog = buf;

			Read(DRVRegName::OV_VDS_FAULT, &buf);
			DRVRegisters.mOVVDSFault = buf;

			Read(DRVRegName::IC_FAULT, &buf);
			DRVRegisters.mICFault = buf;

			Read(DRVRegName::VGS_FAULT, &buf);
			DRVRegisters.mVGSFault = buf;

			Read(DRVRegName::HS_GATE_DRIVER_CTRL, &buf);
			DRVRegisters.mHSDriver = buf;

			Read(DRVRegName::LS_GATE_DRIVER_CTRL, &buf);
			DRVRegisters.mLSDriver = buf;

			Read(DRVRegName::GATE_DRIVE_CTRL, &buf);
			DRVRegisters.mGateDriver = buf;

			Read(DRVRegName::IC_OPERATION, &buf);
			DRVRegisters.mICOparation = buf;

			Read(DRVRegName::SHUNT_AMPLIFIER_CTRL, &buf);
			DRVRegisters.mShuntAmp = buf;

			Read(DRVRegName::VOLTAGE_REGURATOR_CTRL, &buf);
			DRVRegisters.mVolRegurator = buf;

			Read(DRVRegName::VDS_SENSE_CTRL, &buf);
			DRVRegisters.mVDSSense = buf;

		}

		std::string Observe(){
			static std::string msg;
			msg.reserve(256);
			msg.clear();


			if(!IsFault()){
				msg.append('\0');
				return msg;
			}

			msg.append("Fault");

			if(IsOverCurrent())
				msg.append("reason: over current");
			if(IsOverCurrentHA())
				msg.append("reason: high side FET-A is over current");
			if(IsOverCurrentLA())
				msg.append("reason: low side FET-A is over current");
			if(IsOverCurrentHB())
				msg.append("reason: high side FET-B is over current");
			if(IsOverCurrentLB())
				msg.append("reason: low side FET-B is over current");
			if(IsOverCurrentHC())
				msg.append("reason: high side FET-C is over current");
			if(IsOverCurrentLC())
				msg.append("reason: low side FET-C is over current");
			if(IsPVDDLow())
				msg.append("reason: PVDD low");
			if(IsWatchDog())
				msg.append("reason: watch dog");
			if(IsOverTemperature())
				msg.append("reason: high temperature");
			if(IsVREGLow())
				msg.append("reason: VREG is low voltage");
			if(IsAVDDLow())
				msg.append("reason: AVDD is low voltage");
			if(IsLSSupply())
				msg.append("reason: low side gate supply is low");
			if(IsCHPLow())
				msg.append("reason: charge pump voltage is low");
			if(IsCHPHigh())
				msg.append("reason: charge pump voltage is high");
			if(IsCHPHighABS())
				msg.append("reason: charge pump voltage is high with ABS");
			if(IsVGSHA())
				msg.append("reason: gate drive fault for high side FET-A");
			if(IsVGSLA())
				msg.append("reason: gate drive fault for low side FET-A");
			if(IsVGSHB())
				msg.append("reason: gate drive fault for high side FET-B");
			if(IsVGSLA())
				msg.append("reason: gate drive fault for low side FET-B");
			if(IsVGSHC())
				msg.append("reason: gate drive fault for high side FET-C");
			if(IsVGSLC())
				msg.append("reason: gate drive fault for low side FET-C");

			return msg;
		}

		bool IsFault() {
			WARNING_WATCHDOG_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.FAULT;
		}

		bool WarnHighTempereture() {
			WARNING_WATCHDOG_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return (var.TEMP123 | var.TEMP4 | var.OTW);
		}

		bool WarnPVDDLow() {
			WARNING_WATCHDOG_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.PVDD_UV;
		}

		bool WarnPVDDHigh() {
			WARNING_WATCHDOG_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.PVDD_OV;
		}

		bool WarnCHPLow() {
			WARNING_WATCHDOG_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.CHP_UV;
		}

		bool IsOverCurrent() {
			WARNING_WATCHDOG_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.VDS_STATUS;
		}

		bool IsOverCurrentHA() {
			OV_VDS_FAULT_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.VDS_HA;
		}

		bool IsOverCurrentLA() {
			OV_VDS_FAULT_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.VDS_LA;
		}

		bool IsOverCurrentHB() {
			OV_VDS_FAULT_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.VDS_HB;
		}

		bool IsOverCurrentLB() {
			OV_VDS_FAULT_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.VDS_LB;
		}

		bool IsOverCurrentHC() {
			OV_VDS_FAULT_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.VDS_HC;
		}

		bool IsOverCurrentLC() {
			OV_VDS_FAULT_T var;
			var = GetRegValue(DRVRegName::WARNING_WATCHDOG);
			return var.VDS_LC;
		}

		bool IsPVDDLow() {
			IC_FAULT_T var;
			var = GetRegValue(DRVRegName::IC_FAULT);
			return var.PVDD_UVLO2;
		}

		bool IsWatchDog() {
			IC_FAULT_T var;
			var = GetRegValue(DRVRegName::IC_FAULT);
			return var.WD_FAULT;
		}

		bool IsOverTemperature() {
			IC_FAULT_T var;
			var = GetRegValue(DRVRegName::IC_FAULT);
			return var.OTSD;
		}

		bool IsVREGLow() {
			IC_FAULT_T var;
			var = GetRegValue(DRVRegName::IC_FAULT);
			return var.VREG_UV;
		}

		bool IsAVDDLow() {
			IC_FAULT_T var;
			var = GetRegValue(DRVRegName::IC_FAULT);
			return var.AVDD_UVLO;
		}

		bool IsLSSupply() {
			IC_FAULT_T var;
			var = GetRegValue(DRVRegName::IC_FAULT);
			return var.VCP_LSD_UVLO2;
		}

		bool IsCHPLow() {
			IC_FAULT_T var;
			var = GetRegValue(DRVRegName::IC_FAULT);
			return var.VCPH_UVLO2;
		}

		bool IsCHPHigh() {
			IC_FAULT_T var;
			var = GetRegValue(DRVRegName::IC_FAULT);
			return var.VCPH_OVLO;
		}

		bool IsCHPHighABS() {
			IC_FAULT_T var;
			var = GetRegValue(DRVRegName::IC_FAULT);
			return var.VCPH_OVLO_ABS;
		}

		bool IsVGSHA() {
			VGS_FAULT_T var;
			var = GetRegValue(DRVRegName::VGS_FAULT);
			return var.VGS_HA;
		}

		bool IsVGSLA() {
			VGS_FAULT_T var;
			var = GetRegValue(DRVRegName::VGS_FAULT);
			return var.VGS_LA;
		}

		bool IsVGSHB() {
			VGS_FAULT_T var;
			var = GetRegValue(DRVRegName::VGS_FAULT);
			return var.VGS_HB;
		}

		bool IsVGSLB() {
			VGS_FAULT_T var;
			var = GetRegValue(DRVRegName::VGS_FAULT);
			return var.VGS_LB;
		}

		bool IsVGSHC() {
			VGS_FAULT_T var;
			var = GetRegValue(DRVRegName::VGS_FAULT);
			return var.VGS_HC;
		}

		bool IsVGSLC() {
			VGS_FAULT_T var;
			var = GetRegValue(DRVRegName::VGS_FAULT);
			return var.VGS_LC;
		}

		void ClearFault(){
			IC_OPERATION_T var;
			var = GetRegValue(DRVRegName::IC_OPERATION);
			var.CLR_FLTS = CLEAR_FAULTS;
			SetConfiguration(DRVRegName::IC_OPERATION, var);
		}
	}
}

