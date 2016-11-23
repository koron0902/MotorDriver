#ifndef COMMON_INC_TYPE_HPP_
#define COMMON_INC_TYPE_HPP_

#include <fix.hpp>
#include <functional>
#include <stdint.h>
#include <string>
#include <vector>



namespace common {
//コールバック関数の型
using callback=std::function<void(void)>;
using callback_ref=std::function<void(void)>&;
//Shellのコマンド
using ShellParameter=std::vector<std::string>;
using command=std::function<std::string(std::vector<std::string>&)>;

using q32_t = uint32_t;
using q31_t =int32_t;
using q16_t =uint16_t;
using q15_t=int16_t;
using q8_t =uint8_t;
using q7_t =int8_t;

using uint=unsigned int;
}

#endif /* COMMON_INC_TYPE_HPP_ */
