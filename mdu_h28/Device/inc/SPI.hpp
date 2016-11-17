/*
 * SPI.h
 *
 *  Created on: 2016/11/07
 *      Author: Keita
 */

#ifndef DRIVER_INC_SPI_H_
#define DRIVER_INC_SPI_H_

namespace Device{
	namespace SPI{

		void Init();

		bool ReadWrite(uint16_t* txbuf, uint16_t* rxbuf, const uint8_t len = 1, const uint8_t size = 16);

		void EnableSPI();

		void DisableSPI();
	}

}
#endif /* DRIVER_INC_SPI_H_ */
