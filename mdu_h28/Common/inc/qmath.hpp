#ifndef COMMON_INC_QMATH_HPP_
#define COMMON_INC_QMATH_HPP_

#include <bits.hpp>
#include <type.hpp>

namespace common {

template<class T> T abs(const T& a) {
	if (a >= T(0)) {
		return a;
	} else {
		return -a;
	}
}


//負数なら1
template<class T> bool sign(const T& c) {
	return c <T(0);
}

constexpr int fact(int x);//階乗

constexpr fix32 pow(fix32 x,uint n);//x^nを求める

fix32 exp(fix32);//自然対数の底
fix32 sin(fix32);
fix32 cos(fix32);



}

#endif /* COMMON_INC_QMATH_HPP_ */
