/*
 * SPI.cpp
 *
 *  Created on: 2016/11/07
 *      Author: Keita
 */

#include <chip.h>
#include <Port.hpp>
#include <SPI.hpp>
#include <unit.hpp>

// 名前の省略
#define SPI0 LPC_SPI0

namespace Device{
	namespace SPI{

		using namespace common;
		static constexpr uint32_t SYSTEM_CORE_CLOCK        = 72_MHz;
		static constexpr uint32_t SPI_BIT_RATE             = 8_MHz;
		static constexpr uint16_t SPI_CLOCK_DIVIDER        = (SYSTEM_CORE_CLOCK / SPI_BIT_RATE) - 1;
		static constexpr uint8_t  SPI_PRE_DELAY_CYCLE      = 2;
		static constexpr uint8_t  SPI_POST_DELAY_CYCLE     = 2;
		static constexpr uint8_t  SPI_FRAME_DELAY_CYCLE    = 2;
		static constexpr uint8_t  SPI_TRANSFER_DELAY_CYCLE = 2;

		/* * * * * * * * * * * * * * * * * *
		 * summary : Enable SPI module     *
		 * arg     : none                  *
		 * return  : none                  *
		 * * * * * * * * * * * * * * * * * */
		void EnableSPI(){
			SPI0->CFG |= SPI_CFG_SPI_EN;
		}

		/* * * * * * * * * * * * * * * * * *
		 * summary : Disable SPI module    *
		 * arg     : none                  *
		 * return  : none                  *
		 * * * * * * * * * * * * * * * * * */
		void DisableSPI(){
			SPI0->CFG &= (~SPI_CFG_SPI_EN) & SPI_CFG_BITMASK;
		}

		/* * * * * * * * * * * * * * * * * *
		 * summary : Initialize SPI module *
		 * arg     : none                  *
		 * return  : none                  *
		 * * * * * * * * * * * * * * * * * */
		void Init(){
			Chip_SPI_Init(SPI0);

			// Running mode configuration for SPI0
			static SPI_CFG_T SpiCfg;
			SpiCfg.ClkDiv = SPI_CLOCK_DIVIDER;
			SpiCfg.Mode = SPI_MODE_MASTER;
			SpiCfg.ClockMode = SPI_CLOCK_MODE1;
			SpiCfg.DataOrder = SPI_DATA_MSB_FIRST;
			SpiCfg.SSELPol = SPI_CFG_SPOL0_LO |
							 SPI_CFG_SPOL1_LO |
							 SPI_CFG_SPOL2_LO |
							 SPI_CFG_SPOL3_LO;
			Chip_SPI_SetConfig(SPI0, &SpiCfg);

			// Delay configuration  for SPI0
			static SPI_DELAY_CONFIG_T SpiDelayConf;
			SpiDelayConf.PreDelay = SPI_PRE_DELAY_CYCLE;
			SpiDelayConf.PostDelay = SPI_POST_DELAY_CYCLE;
			SpiDelayConf.FrameDelay = SPI_FRAME_DELAY_CYCLE;
			SpiDelayConf.TransferDelay = SPI_TRANSFER_DELAY_CYCLE;
			Chip_SPI_DelayConfig(SPI0, &SpiDelayConf);

			// Enable SPI0 module
			EnableSPI();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 * summary      : transmit and receive data via SPI            *
		 * arg1 - txbuf : pointer to transmit buffer                   *
		 * arg2 - rxbuf : pointer to buffer to receive                 *
		 * arg3 - len   : number of frames(default: 1)                 *
		 * arg4 - size  : number of bits in unit of frame(default: 16) *
		 * return       : true -> success, false -> failure            *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		bool ReadWrite(uint16_t* txbuf, uint16_t* rxbuf, const uint8_t len, const uint8_t size){
			static SPI_DATA_SETUP_T SpiData;

			SpiData.pTx = txbuf;
			SpiData.pRx = rxbuf;
			SpiData.DataSize = size;
			SpiData.Length = len;
			SpiData.ssel = SPI_TXCTL_ASSERT_SSEL0 | SPI_TXCTL_DEASSERT_SSEL1 | SPI_TXCTL_DEASSERT_SSEL2 | SPI_TXCTL_DEASSERT_SSEL3;
			SpiData.TxCnt = 0;
			SpiData.RxCnt = 0;

			uint32_t retVal = Chip_SPI_RWFrames_Blocking(SPI0, &SpiData);
			if(retVal == 0){
				// error code
				return false;
			}

			// data reform to drv8305 format
			for (int i = 0;i < len;i++){
				*(rxbuf + i) &= 0x07FF;
			}

			return true;
		}

	}

}
