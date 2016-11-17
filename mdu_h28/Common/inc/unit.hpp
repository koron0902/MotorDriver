#ifndef COMMON_INC_UNIT_HPP_
#define COMMON_INC_UNIT_HPP_

#include <stdint.h>

namespace common{
//補助単位群(ただし次元を考慮しない)
//ここエラー表示出るけど問題ない。
static inline constexpr int32_t operator ""_Hz(unsigned long long value){
	return value;
}
static inline constexpr int32_t operator ""_KHz(unsigned long long value){
	return value*1000;
}
static inline constexpr int32_t operator ""_MHz(unsigned long long value){
	return value*1000000;
}

static inline constexpr int32_t operator ""_Byte(unsigned long long value){
	return value*1024;
}

static inline constexpr int32_t operator ""_KiByte(unsigned long long value){
	return value*1024;
}

static inline constexpr int32_t operator ""_MiByte(unsigned long long value){
	return value*1024*1024;
}


}
#endif /* COMMON_INC_UNIT_HPP_ */
