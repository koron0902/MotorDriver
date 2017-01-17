#pragma once
#ifndef COMMON_INC_STATUS_HPP_
#define COMMON_INC_STATUS_HPP_

#include <type.hpp>

namespace common {
/*
//DRVのデータシートを参照のこと。
enum class PWMMode
	:uint32_t {
		AB = 0b0110,
	AB_CB = 0b0101,
	CB = 0b0100,
	CB_CA = 0b1101,
	CA = 0b1100,
	CA_BA = 0b1001,
	BA = 0b1000,
	BA_BC = 0b1011,
	BC = 0b1010,
	BC_AC = 0b0011,
	AC = 0b0010,
	AC_AB = 0b0111,
	Align = 0b1110,
	Stop = 0b0000,
	Halt = Stop
};
*/

//内部で計算するようのモーターの位置
enum class MotorPosition
	:uint32_t {
		None = 0b000,
	U = 0b001,
	UV = 0b011,
	V = 0b010,
	VW = 0b110,
	W = 0b100,
	WU = 0b101
};

//次の場所を計算する。
constexpr MotorPosition Next(MotorPosition);
constexpr MotorPosition Previous(MotorPosition);



}
#endif /* COMMON_INC_STATUS_HPP_ */
