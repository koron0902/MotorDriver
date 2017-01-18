/*
 * DRVRegister.h
 *
 *  Created on: 2016/11/15
 *      Author: Keita
 */

#ifndef MIDDLE_INC_DRVREGISTER_H_
#define MIDDLE_INC_DRVREGISTER_H_


namespace Middle{
	namespace DRV{
		enum class DRVRegName : uint32_t{
			WARNING_WATCHDOG		= 0x01,
			OV_VDS_FAULT			= 0x02,
			IC_FAULT				= 0x03,
			VGS_FAULT				= 0x04,
			HS_GATE_DRIVER_CTRL 	= 0x05,
			LS_GATE_DRIVER_CTRL		= 0x06,
			GATE_DRIVE_CTRL			= 0x07,
			IC_OPERATION			= 0x09,
			SHUNT_AMPLIFIER_CTRL	= 0x0A,
			VOLTAGE_REGURATOR_CTRL	= 0x0B,
			VDS_SENSE_CTRL			= 0x0C
		};

		union WARNING_WATCHDOG_T{
			unsigned d				 : 11;
			struct {
				unsigned OTW		 : 1;
				unsigned TEMP123	 : 3;
				unsigned CHP_UV		 : 1;
				unsigned VDS_STATUS	 : 1;
				unsigned PVDD_OV	 : 1;
				unsigned PVDD_UV	 : 1;
				unsigned TEMP4		 : 1;
				unsigned 			 : 1;
				unsigned FAULT		 : 1;
			};
			operator uint16_t() const{return d;}
			WARNING_WATCHDOG_T operator =(uint16_t value) {d = value; return *this;}
		};

		union OV_VDS_FAULT_T{
			unsigned d				 : 11;
			struct {
				unsigned SNS_A_OCP	 : 1;
				unsigned SNS_B_OCP	 : 1;
				unsigned SNS_C_OCP	 : 1;
				unsigned 			 : 2;
				unsigned VDS_LC		 : 1;
				unsigned VDS_HC		 : 1;
				unsigned VDS_LB		 : 1;
				unsigned VDS_HB		 : 1;
				unsigned VDS_LA		 : 1;
				unsigned VDS_HA		 : 1;
			};
			operator uint16_t() const{return d;}
			OV_VDS_FAULT_T operator = (uint16_t value) {d = value; return *this;}
		};

		union IC_FAULT_T{
			unsigned d					 : 11;
			struct {
				unsigned VCPH_OVLO_ABS	 : 1;
				unsigned VCPH_OVLO		 : 1;
				unsigned VCPH_UVLO2		 : 1;
				unsigned 			 	 : 1;
				unsigned VCP_LSD_UVLO2	 : 1;
				unsigned AVDD_UVLO 		 : 1;
				unsigned VREG_UV		 : 1;
				unsigned				 : 1;
				unsigned OTSD			 : 1;
				unsigned WD_FAULT		 : 1;
				unsigned PVDD_UVLO2		 : 1;
			};
			operator uint16_t() const{return d;}
			IC_FAULT_T operator = (uint16_t value) {d = value; return *this;}
		};

		union VGS_FAULT_T{
			unsigned d					 : 11;
			struct {
				unsigned				 : 5;
				unsigned VGS_LC			 : 1;
				unsigned VGS_HC			 : 1;
				unsigned VGS_LB			 : 1;
				unsigned VGS_HB			 : 1;
				unsigned VGS_LA			 : 1;
				unsigned VGS_HA			 : 1;
			};
			operator uint16_t() const{return d;}
			VGS_FAULT_T operator = (uint16_t value) {d = value; return *this;}
		};

		/* * * * * * Gate Driverに関するお話 * * * * * */
		typedef enum PEAK_SINK_MODE{
			PEAK_SOURCE_20mA	= 0b0000,
			PEAK_SOURCE_30mA	= 0b0001,
			PEAK_SOURCE_40mA	= 0b0010,
			PEAK_SOURCE_50mA	= 0b0011,
			PEAK_SOURCE_60mA	= 0b0100,
			PEAK_SOURCE_70mA	= 0b0101,
			PEAK_SOURCE_80mA	= 0b0110,
			PEAK_SOURCE_250mA	= 0b0111,
			PEAK_SOURCE_500mA	= 0b1000,
			PEAK_SOURCE_750mA	= 0b1001,
			PEAK_SOURCE_1000mA	= 0b1010,
			PEAK_SOURCE_1250mA	= 0b1011,
			PEAK_SOURCE_DEFAULT = PEAK_SOURCE_60mA,
			PEAK_SOURCE_MIN		= PEAK_SOURCE_20mA,
			PEAK_SOURCE_MAX		= PEAK_SOURCE_1250mA
		}PEAK_SINK_CURRENT_T;

		typedef enum PEAK_SOURCE_MODE{
			PEAK_SINK_10mA		= 0b0000,
			PEAK_SINK_20mA		= 0b0001,
			PEAK_SINK_30mA		= 0b0010,
			PEAK_SINK_40mA		= 0b0011,
			PEAK_SINK_50mA		= 0b0100,
			PEAK_SINK_60mA		= 0b0101,
			PEAK_SINK_70mA		= 0b0110,
			PEAK_SINK_125mA		= 0b0111,
			PEAK_SINK_250mA		= 0b1000,
			PEAK_SINK_500mA		= 0b1001,
			PEAK_SINK_750mA		= 0b1010,
			PEAK_SINK_1000mA	= 0b1011,
			PEAK_SINK_DEFAULT 	= PEAK_SINK_50mA,
			PEAK_SINK_MIN		= PEAK_SINK_10mA,
			PEAK_SINK_MAX		= PEAK_SINK_1000mA
		}PEAK_SOURCE_CURRENT_T;

		typedef enum SOURCE_TIME_MODE{
			SOURCE_TIME_220ns	= 0b00,
			SOURCE_TIME_440ns	= 0b01,
			SOURCE_TIME_880ns	= 0b10,
			SOURCE_TIME_1780ns	= 0b11,
			SOURCE_TIME_DEFAULT = SOURCE_TIME_1780ns
		}PEAK_SOURCE_TIME_T;

		union HS_GATE_DRIVER_CTRL_T{
			unsigned d								: 11;
			struct {
				PEAK_SINK_CURRENT_T IDRIVEP_HS		: 4;
				PEAK_SOURCE_CURRENT_T IDRIVEN_HS	: 4;
				PEAK_SOURCE_TIME_T TDRIVEN			: 2;
				unsigned 				 			: 1;
			};

			HS_GATE_DRIVER_CTRL_T(){
				IDRIVEP_HS	= PEAK_SOURCE_DEFAULT;
				IDRIVEN_HS	= PEAK_SINK_DEFAULT;
				TDRIVEN		= SOURCE_TIME_DEFAULT;
			}

			operator uint16_t() const{return d;}
			HS_GATE_DRIVER_CTRL_T operator = (uint16_t value) {d = value; return *this;}
		};

		union LS_GATE_DRIVER_CTRL_T{
			unsigned d					 			: 11;
			struct {
				PEAK_SINK_CURRENT_T IDRIVEP_LS		: 4;
				PEAK_SOURCE_CURRENT_T IDRIVEN_LS	: 4;
				PEAK_SOURCE_TIME_T TDRIVEP			: 2;
				unsigned				 			: 1;
			};

			LS_GATE_DRIVER_CTRL_T(){
				IDRIVEP_LS	= PEAK_SOURCE_DEFAULT;
				IDRIVEN_LS	= PEAK_SINK_DEFAULT;
				TDRIVEP		= SOURCE_TIME_DEFAULT;
			}

			operator uint16_t() const{return d;}
			LS_GATE_DRIVER_CTRL_T operator = (uint16_t value) {d = value; return *this;}
		};

		/* * * * * * Gateのドライブ方法に関するお話 * * * * * */
		typedef enum VDS_SENSE_DEGLITCH_MODE{
			DEGLIGTCH_TIME_0		= 0b00,
			DEGLITCH_TIME_1u75s		= 0b01,
			DEGLITCH_TIME_3u5s		= 0b10,
			DEGLITCH_TIME_7us		= 0b11,
			DEGLITCH_TIME_DEFAULT	= DEGLITCH_TIME_3u5s
		}VDS_DEGLITCH_TIME_T;

		typedef enum VDS_SENSE_BLANK_MODE{
			SENSE_BLANK_TIME_0			= 0b00,
			SENSE_BLANK_TIME_1u75s		= 0b01,
			SENSE_BLANK_TIME_3u5s		= 0b10,
			SENSE_BLANK_TIME_7us		= 0b11,
			SENSE_BLANK_TIME_DEFAULT	= SENSE_BLANK_TIME_1u75s
		}VDS_BLANK_TIME_T;

		typedef enum DEAD_TIME_MODE{
			DEAD_TIME_35ns		= 0b000,
			DEAD_TIME_52ns		= 0b001,
			DEAD_TIME_88ns		= 0b010,
			DEAD_TIME_440ns		= 0b011,
			DEAD_TIME_880ns		= 0b100,
			DEAD_TIME_1760ns	= 0b101,
			DEAD_TIME_3520ns	= 0b110,
			DEAD_TIME_5280ns	= 0b111,
			DEAD_TIME_DEFAULT	= DEAD_TIME_52ns
		}DEAD_TIME_T;

		typedef enum PWM_USES_MODE{
			PWM_USES_6		= 0b00,
			PWM_USES_3		= 0b01,
			PWM_USES_1		= 0b10,
			PWM_USES_DEFAULT	= PWM_USES_6
		}PWM_USES_T;

		typedef enum RECTIFICATION_MODE{
			FREE_WHEEL_DIODE	= 0b0,
			FREE_WHEEL_ACTIVE	= 0b1,
			FREE_WHEEL_DEFAULT	= FREE_WHEEL_ACTIVE
		}RECTIFICATION_T;

		union GATE_DRIVE_CTRL_T{
			unsigned d						: 11;
			struct {
				VDS_DEGLITCH_TIME_T TVDS	: 2;
				VDS_BLANK_TIME_T TBLANK		: 2;
				DEAD_TIME_T DEAD_TIME		: 3;
				PWM_USES_T PWM_MODE		 	: 2;
				RECTIFICATION_T COMM_OPTION	: 1;
				unsigned					: 1;
			};

			GATE_DRIVE_CTRL_T(){
				TVDS		= DEGLITCH_TIME_DEFAULT;
				TBLANK		= SENSE_BLANK_TIME_DEFAULT;
				DEAD_TIME	= DEAD_TIME_DEFAULT;
				PWM_MODE	= PWM_USES_DEFAULT;
				COMM_OPTION	= FREE_WHEEL_DEFAULT;
			}

			operator uint16_t() const{
				return (COMM_OPTION << 9 |
						PWM_MODE << 7 |
						DEAD_TIME << 4 |
						TBLANK << 2 |
						TVDS
						);
			}
			GATE_DRIVE_CTRL_T operator = (uint16_t value) {d = value; return *this;}
		};

		/* * * * * * ICの動作モードに関するお話 * * * * * */
		typedef enum CHP_UV_THRESHOLD_MODE{
			 CHP_UV_THRESHOLD_4v9		= 0b0,
			 CHP_UV_THRESHOLD_4v6		= 0b1,
			 CHP_UV_THRESHOLD_DEFAULT	= CHP_UV_THRESHOLD_4v9,
		}CHP_THRESHOLD_LEVEL_T;

		typedef enum CLEAR_FAULT_BIT{
			NOMAL_OPERATION				= 0b0,
			CLEAR_FAULTS				= 0b1,
			CLEAR_FAULTS_BIT_DEFAULT	= NOMAL_OPERATION
		}CLEAR_FAULT_BIT_T;

		typedef enum DEVICE_RUNNING_MODE{
			DEVICE_PUT_AWAKE	= 0b0,
			DEVICE_PUT_ASLEEP	= 0b1,
			DEVICE_PUT_DEFAULT	= DEVICE_PUT_AWAKE,
		}DEVICE_RUNNING_MODE_T;

		typedef enum WATCH_DOG_MODE{
			WATCH_DOG_DISABLE	= 0b0,
			WATCH_DOG_ENABLE	= 0b1,
			WATCH_DOG_DEFAULT	= WATCH_DOG_DISABLE
		}WATCH_DOG_ENABLE_T;

		typedef enum OVER_CURRENT_PROTECTION_MODE{
			SNS_OCP_ENABLE		= 0b0,
			SNS_OCP_DISABLE		= 0b1,
			SNS_OCP_DEFAULT		= SNS_OCP_ENABLE,
		}SNS_OCP_ENABLE_T;

		typedef enum WATCH_DOG_DELAY_MODE{
			WATCH_DOG_DELAY_10ms	= 0b00,
			WATCH_DOG_DELAY_20ms	= 0b01,
			WATCH_DOG_DELAY_50ms	= 0b10,
			WATCH_DOG_DELAY_100ms	= 0b11,
			WATCH_DOG_DELAY_DEFAULT	= WATCH_DOG_DELAY_20ms
		}WATCH_DOG_DELAY_T;

		typedef enum SENSE_AMP_CLAMP_MODE{
			AMPLIFIER_CLAMP_DISABLED	= 0b0,
			AMPLIFIER_CLAMP_ENABLED		= 0b1,
			AMPLIFIER_CLAMP_DEFAULT		= AMPLIFIER_CLAMP_DISABLED
		}CLAMP_ENABLE_T;

		typedef enum GD_FAULT_MODE {
			GATE_DRIVER_FAULT_ENABLE	= 0b0,
			GATE_DRIVER_FAULT_DISABLE	= 0b1,
			GATE_DRIVER_FAULT_DEFAULT	= GATE_DRIVER_FAULT_ENABLE
		}GATE_DRIVER_FAULT_T;

		typedef enum PVDD_FAULT_MODE {
			PVDD_UVLO2_FAULT_ENABLE		= 0b0,
			PVDD_UVLO2_FAULT_DISABLE	= 0b1,
			PVDD_UVLO2_FAULT_DEFAULT	= PVDD_UVLO2_FAULT_ENABLE
		}PVDD_UVLO2_FAULT_T;

		typedef enum OTSD_MODE{
			OTSD_DISABLE	= 0b0,
			OTSD_ENABLE		= 0b1,
			OTSD_DEFAULT	= OTSD_ENABLE
		}OTSD_ENABLE_T;

		union IC_OPERATION_T{
			unsigned d					 : 11;
			struct {
				CHP_THRESHOLD_LEVEL_T SET_VCPH_UV	: 1;
				CLEAR_FAULT_BIT_T CLR_FLTS			: 1;
				DEVICE_RUNNING_MODE_T SLEEP			: 1;
				WATCH_DOG_ENABLE_T WD_EN			: 1;
				SNS_OCP_ENABLE_T DIS_SNS_OCP		: 1;
				WATCH_DOG_DELAY_T WD_DLY			: 2;
				CLAMP_ENABLE_T EN_SNS_CLAMP	 		: 1;
				GATE_DRIVER_FAULT_T DIS_GDRV_FAULT  : 1;
				PVDD_UVLO2_FAULT_T DIS_PVDD_UVLO2	: 1;
				OTSD_ENABLE_T FLIP_OTSD				: 1;
			};

			IC_OPERATION_T(){
				SET_VCPH_UV		= CHP_UV_THRESHOLD_DEFAULT;
				CLR_FLTS		= CLEAR_FAULTS_BIT_DEFAULT;
				SLEEP			= DEVICE_PUT_DEFAULT;
				WD_EN			= WATCH_DOG_DEFAULT;
				DIS_SNS_OCP		= SNS_OCP_DEFAULT;
				WD_DLY			= WATCH_DOG_DELAY_DEFAULT;
				EN_SNS_CLAMP	= AMPLIFIER_CLAMP_DEFAULT;
				DIS_GDRV_FAULT	= GATE_DRIVER_FAULT_DEFAULT;
				DIS_PVDD_UVLO2	= PVDD_UVLO2_FAULT_DEFAULT;
				FLIP_OTSD		= OTSD_DEFAULT;
			}

			operator uint16_t() const{return d;}
			IC_OPERATION_T operator = (uint16_t value) {d = value; return *this;}
		};

		/* * * * * * 電流センサのゲインに関するお話 * * * * * */
		typedef enum GAIN_MODE{
			GAIN_10			= 0b00,
			GAIN_20			= 0b01,
			GAIN_40			= 0b10,
			GAIN_80			= 0b11,
			GAIN_DEFAULT	= GAIN_10
		}GAIN_T;

		typedef enum CURRENT_AMP_BLANK_MODE{
			AMP_BLANK_TIME_0 		= 0b00,
			AMP_BLANK_TIME_500ns	= 0b01,
			AMP_BLANK_TIME_2u5s		= 0b01,
			AMP_BLANK_TIME_10s		= 0b01,
			AMP_BLANK_TIME_DEFAULT	= AMP_BLANK_TIME_0
		}AMP_BLANK_TIME_T;

		typedef enum CALIBRATION_MODE{
			DC_NOMAL_OPERATION		= 0b0,
			DC_CALIBRATION			= 0b1,
			CALIBRATION_DEFAULT		= DC_NOMAL_OPERATION
		}DC_CALIBRATION_T;

		union SHUNT_AMPLIFIER_CTRL_T{
			unsigned d					 		: 11;
			struct {
				GAIN_T GAIN_CS1		 			: 2;
				GAIN_T GAIN_CS2		 			: 2;
				GAIN_T GAIN_CS3		 			: 2;
				AMP_BLANK_TIME_T CS_BLANK		: 2;
				DC_CALIBRATION_T DC_CAL_CH1		: 1;
				DC_CALIBRATION_T DC_CAL_CH2		: 1;
				DC_CALIBRATION_T DC_CAL_CH3		: 1;
			};

			SHUNT_AMPLIFIER_CTRL_T(){
				GAIN_CS1	= GAIN_DEFAULT;
				GAIN_CS2	= GAIN_DEFAULT;
				GAIN_CS3	= GAIN_DEFAULT;
				CS_BLANK	= AMP_BLANK_TIME_DEFAULT;
				DC_CAL_CH1	= CALIBRATION_DEFAULT;
				DC_CAL_CH2	= CALIBRATION_DEFAULT;
				DC_CAL_CH3	= CALIBRATION_DEFAULT;
			}

			operator uint16_t() const{return d;}
			SHUNT_AMPLIFIER_CTRL_T operator = (uint16_t value) {d = value; return *this;}
		};

		/* * * * * * レギュレータに関するお話 * * * * * */
		typedef enum VREG_UV_LEVEL_MODE{
			VREG_TO_90		= 0b00,
			VREG_TO_80		= 0b01,
			VREG_TO_70		= 0b10,
			DEFAULT_LEVEL	= VREG_TO_70
		}VREG_UV_LEVEL_T;

		typedef enum VREG_UV_FAULT_MODE{
			VREG_UV_FAULT_ENABLE	= 0b0,
			VREG_UV_FAULT_DISABLE	= 0b1,
			VREG_UV_FAULT_DEFAULT	= VREG_UV_FAULT_ENABLE,
		}VREG_UV_FAULT_T;

		typedef enum SLEEP_DELAY_MODE{
			SLEEP_DELAY_0		= 0b00,
			SLEEP_DELAY_10us	= 0b01,
			SLEEP_DELAY_50us	= 0b10,
			SLEEP_DELAY_1ms		= 0b11,
			SLEEP_DELAY_DEFAULT	= SLEEP_DELAY_10us
		}SLEEP_DELAY_T;

		typedef enum VREF_SCALE_MODE{
			VREF_SCALE_2		= 0b01,
			VREF_SCALE_4		= 0b10,
			VREF_SCALE_DEFAULT	= VREF_SCALE_2
		}VREF_SCALE_T;

		union VOLTAGE_REGURATOR_CTRL_T{
			unsigned d					 		: 11;
			struct {
				VREG_UV_LEVEL_T VREG_UV_LEVEL	: 2;
				VREG_UV_FAULT_T DIS_VREG_PWRGD	: 1;
				SLEEP_DELAY_T SLEEP_DLY			: 2;
				unsigned 						: 3;
				VREF_SCALE_T VREF_SCALE			: 2;
				unsigned 						: 1;
			};

			VOLTAGE_REGURATOR_CTRL_T(){
				VREG_UV_LEVEL	= DEFAULT_LEVEL;
				DIS_VREG_PWRGD	= VREG_UV_FAULT_DEFAULT;
				SLEEP_DLY		= SLEEP_DELAY_DEFAULT;
				VREF_SCALE		= VREF_SCALE_DEFAULT;
			}

			operator uint16_t() const{return d;}
			VOLTAGE_REGURATOR_CTRL_T operator = (uint16_t value) {d = value; return *this;}
		};

		/* * * * * * コンパレータに関するお話 * * * * * */
		typedef enum VDS_PROTECTION_MODE{
			SHUT_DOWN			= 0b000,
			REPORT_ONLY			= 0b001,
			PROTECTION_DISABLE	= 0b010,
			PROTECTION_DEFAULT	= SHUT_DOWN
		}VDS_PROTECTION_T;

		typedef enum COMPARATOR_THRESHOLD_MODE{
			THRESHOLD_0V060		= 0b00000,
			THRESHOLD_0V068		= 0b00001,
			THRESHOLD_0V076		= 0b00010,
			THRESHOLD_0V086		= 0b00011,
			THRESHOLD_0V097		= 0b00100,
			THRESHOLD_0V109		= 0b00101,
			THRESHOLD_0V123		= 0b00110,
			THRESHOLD_0V138		= 0b00111,
			THRESHOLD_0V155		= 0b01000,
			THRESHOLD_0V175		= 0b01001,
			THRESHOLD_0V197		= 0b01010,
			THRESHOLD_0V222		= 0b01011,
			THRESHOLD_0V250		= 0b01100,
			THRESHOLD_0V282		= 0b01101,
			THRESHOLD_0V317		= 0b01110,
			THRESHOLD_0V358		= 0b01111,
			THRESHOLD_0V403		= 0b10000,
			THRESHOLD_0V454		= 0b10001,
			THRESHOLD_0V511		= 0b10010,
			THRESHOLD_0V576		= 0b10011,
			THRESHOLD_0V648		= 0b10100,
			THRESHOLD_0V730		= 0b10101,
			THRESHOLD_0V822		= 0b10110,
			THRESHOLD_0V926		= 0b10111,
			THRESHOLD_1V043		= 0b11000,
			THRESHOLD_1V175		= 0b11001,
			THRESHOLD_1V324		= 0b11010,
			THRESHOLD_1V491		= 0b11011,
			THRESHOLD_1V679		= 0b11100,
			THRESHOLD_1V892		= 0b11101,
			THRESHOLD_2V131		= 0b11110,
			THRESHOLD_DEFAULT 	= THRESHOLD_1V175
		}COMPARATOR_THRESHOLD_T;

		union VDS_SENSE_CTRL_T{
			unsigned d					 			: 11;
			struct {
				VDS_PROTECTION_T VDS_MODE			: 3;
				COMPARATOR_THRESHOLD_T VDS_LEVEL	: 5;
				unsigned 							: 3;
			};

			VDS_SENSE_CTRL_T(){
				VDS_MODE	= PROTECTION_DEFAULT;
				VDS_LEVEL	= THRESHOLD_DEFAULT;
			}

			operator uint16_t() const{return d;}
			VDS_SENSE_CTRL_T operator = (uint16_t value) {d = value; return *this;}
		};
	}
}


#endif /* MIDDLE_INC_DRVREGISTER_H_ */
