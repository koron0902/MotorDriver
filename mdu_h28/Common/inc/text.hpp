#ifndef COMMON_INC_TEXT_HPP_
#define COMMON_INC_TEXT_HPP_

#include <fix.hpp>
#include <string>
#include <vector>

namespace common {

	constexpr char newline ='\r';

	std::vector<std::string> Split(const std::string& target,const std::string& sep="\r");

	std::string Space(const std::string& str,unsigned int s=0);//cがs個繰り返された文字列

	std::string ToStr(int64_t);


	common::fix32 ToFix(const std::string&);
	int32_t ToInt(const std::string&);


	//トークン処理用
	static inline bool IsNumber(char c){
		return '0'<=c&&c<='9';
	}

	static inline int32_t FromNumber(char c){
		return c-'0';
	}

	static inline bool IsPoint(char c){
		return c=='.';
	}

	static inline bool IsSign(char c){
		return c=='+'||c=='-';
	}

	static inline char ToChar(int32_t value){
		return '0'+value;
	}

} /* namespace common */

#endif /* COMMON_INC_TEXT_HPP_ */
