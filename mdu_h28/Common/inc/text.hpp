#ifndef COMMON_INC_TEXT_HPP_
#define COMMON_INC_TEXT_HPP_

#include <string>
#include <vector>


namespace common {

	constexpr char newline ='\r';

	std::vector<std::string> Split(const std::string& target,const std::string& sep="\r");

	std::string Space(const std::string& str,unsigned int s=0);//cがs個繰り返された文字列

	std::string ToStr(int32_t);


} /* namespace common */

#endif /* COMMON_INC_TEXT_HPP_ */
