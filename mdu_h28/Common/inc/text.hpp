#pragma once
#ifndef COMMON_INC_TEXT_HPP_
#define COMMON_INC_TEXT_HPP_
#include <type.hpp>
#include <fix.hpp>
#include <string>
#include <vector>
#include <matrix.hpp>
#include <fix.hpp>
namespace common {

	constexpr char newline ='\r';

	std::vector<std::string> Split(const std::string& target,const std::string& sep="\r");

	std::string Space(const std::string& str,unsigned int s=0);//cがs個繰り返された文字列

	std::string ToStr(int64_t);
	std::string ToStr(fix32);
	std::string ToStrF(float);


	template <class T,uint X,uint Y>std::string ToStr(const Matrix<T,X,Y>&);

	common::fix32 ToFix(const std::string&);
	int32_t ToInt(const std::string&);

	float ToFloat(const std::string&);


	//トークン処理用
	static inline bool IsNumber(char c){
		return '0'<=c&&c<='9';
	}

	static inline bool IsHex(char c){
		return  ('0'<=c&&c<='9')||('a'<=c&&c<='f')||('A'<=c&&c<='F');
	}

	static inline int32_t FromNumber(char c){
		return c-'0';
	}

	static inline bool IsEnd(char c){
		return c=='\0';
	}

	static inline bool IsPoint(char c){
		return c=='.';
	}

	static inline bool IsSign(char c){
		return c=='+'||c=='-';
	}

	static inline bool IsSmall(char c){
		return 'a'<=c&&c<='z';
	}

	static inline bool IsLarge(char c){
		return 'A'<=c&&c<='Z';
	}

	static inline bool IsAlphabet(char c){
		return IsSmall(c)||IsLarge(c);
	}

	static inline char ToChar(int32_t value){
		return '0'+value;
	}

	static inline char IsMark(char c){
		return ('!'<=c&&c<='/')||(':'<=c&&c<='?')||
				('['<=c&&c<='`')||('{'<=c&&c<='~');
	}

	static inline char IsEndLine(char c){
		return c==newline;
	}

	bool IsNumberPattern(const std::string&);
	/* S:={'+','-'},N:=[0,9],P:={'.'}
	 * (S|0)N#(PN*|0)
	 */

	bool IsOptionPattern(const std::string&);
	 /* A=['a','Z'],N[0,9],C={A|N}
	  * '-'C#
	 */

} /* namespace common */

#include <text.tpp>
#endif /* COMMON_INC_TEXT_HPP_ */
