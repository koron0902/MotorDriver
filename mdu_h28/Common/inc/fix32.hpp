/*
 * fix32.h
 *
 *  Created on: 2016/11/12
 *      Author: hamus
 */

#ifndef COMMON_INC_FIX32_HPP_
#define COMMON_INC_FIX32_HPP_

#pragma once
#include <stdint.h>
//#include <iostream>

namespace common {

struct fix32 {
	//定数部1
	constexpr static unsigned int shift = 16;
	constexpr static int32_t gain = (1 << shift) - 1;
	const static fix32 Max, Min, Eps;
	int32_t word;

	//初期化因子
private:
	fix32() {
		word = 0;
	}
public:
	constexpr fix32(const fix32& x) = default;
	constexpr fix32(int32_t x) :
			word(x) {
	}

	static fix32 CreateRaw(int32_t x) {
		return fix32(x);
	}

	static fix32 CreateInt(int32_t x) {
		return fix32(x << shift);
	}

	static fix32 CreateFloat(float x) {
		return fix32(x * gain);
	}

	int32_t GetRaw() const {
		return word;
	}

	//型変換
	operator float() const {
		return (float) word / gain;
	}

	operator double() const {
		return (double) word / gain;
	}

	operator int32_t() const {
		return word >> shift;
	}

	//整数部を取得する(これは高速)
	int32_t GetInteger() const {
		return word >> shift;
	}
	//小数部を取得する
	int32_t GetPoint() const {
		return word & gain;
	}

	//計算部
	fix32 operator +() const {
		return *this;
	}

	fix32 operator -() const {
		return fix32(-word);
	}

	fix32 operator =(const fix32 x) {
		return fix32(word = x.word);

	}

	fix32 operator +=(const fix32 x) {
		return fix32(word += x.word);
	}

	fix32 operator +=(int32_t x) {
		return fix32(word += (x << shift));
	}

	fix32 operator -=(const fix32 x) {
		return fix32(word -= x.word);
	}

	fix32 operator -=(int32_t x) {
		return fix32(word -= (x << shift));
	}

	fix32 operator *=(const fix32 x) {
		return fix32(word = ((int64_t) word * x.word) >> shift);
	}

	fix32 operator *=(int32_t x) {
		return fix32(word *= x);
	}

	fix32 operator /=(const fix32& x) {
		return fix32(word = (word / x.word) << shift);
	}

	fix32 operator /=(int32_t x) {
		return fix32(word /= x);
	}

	fix32 operator +(const fix32& x) {
		return fix32(word + x.word);
	}

	fix32 operator +(int32_t x) {
		return fix32(word + (x << shift));
	}

	fix32 operator -(const fix32& x) {
		return fix32(word - x.word);
	}

	fix32 operator -(int32_t x) {
		return fix32(word - (x << shift));
	}

	fix32 operator *(const fix32& x) {
		return fix32(((int64_t) word * x.word) >> shift);
	}

	fix32 operator *(int32_t x) {
		return fix32(word * x);
	}

	fix32 operator /(const fix32& x) {
		return fix32(((((int64_t) word) << shift) / x.word));
	}

	fix32 operator /(int32_t x) {
		return fix32(word / x);
	}

	bool operator ==(const fix32& x) const {
		return word == x.word;
	}

	bool operator !=(const fix32& x) const {
		return word != x.word;
	}

	bool operator >(const fix32& x) const {
		return word > x.word;
	}

	bool operator >(int32_t x) const {
		return word > (x << shift);
	}

	bool operator <(const fix32& x) const {
		return word < x.word;
	}

	bool operator <(int32_t x) const {
		return word < (x << shift);
	}

	bool operator >=(const fix32& x) const {
		return word >= x.word;
	}

	bool operator >=(int32_t x) const {
		return word >= (x << shift);
	}

	bool operator <=(const fix32& x) const {
		return word <= x.word;
	}

	bool operator <=(int32_t x) const {
		return word <= (x << shift);
	}

	bool operator !() const {
		return !word;
	}

	fix32 operator ~() const {
		return fix32(~word);
	}

	fix32 operator %(const fix32 x) {
		return fix32(word % x.word);
	}

	fix32 operator <<(int32_t s) const {
		return fix32(word << s);
	}

	fix32 operator >>(int32_t s) const {
		return fix32(word >> s);
	}

	fix32 operator <<=(int32_t s) {
		return fix32(word <<= s);
	}

	fix32 operator >>=(int32_t s) {
		return fix32(word >>= s);
	}

};

}
/*
std::ostream& operator<<(std::ostream& os, const common::fix32 fix);
std::istream& operator >>(std::istream& is, common::fix32&);
*/
#endif /* COMMON_INC_FIX32_HPP_ */
