/*
 * ADC.h
 *
 *  Created on: 2016/11/06
 *      Author: TERU
 */

#ifndef DEVICE_ADC_H_
#define DEVICE_ADC_H_

#include <stdint.h>

namespace Device {

namespace ADC{

uint16_t GetAmpA();
uint16_t GetAmpB();
uint16_t GetAmpC();

uint16_t GetVlot();
uint16_t GetVlotA();
uint16_t GetVlotB();
uint16_t GetVlotC();

void Init();

}

} /* namespace Device */

#endif /* DEVICE_ADC_H_ */
