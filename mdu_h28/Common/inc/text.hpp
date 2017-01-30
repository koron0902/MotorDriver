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

	extern char newline;
	extern char comma;//区切り文字[CSV用]
	std::vector<std::string> Split(const std::string& target,const std::string& sep="\r");

	std::string Space(const std::string& str,unsigned int s=0);//cがs個繰り返された文字列

	std::string ToStr(int64_t);
	std::string ToStr(fix32);
	std::string ToStrF(float);


	template <class T,uint X,uint Y>std::string ToStr(const Matrix<T,X,Y>&);

	common::fix32 ToFix(const std::string&);
	int32_t ToInt32(const std::string&);
	int64_t ToInt64(const std::string&);
	uint32_t ToUInt32(const std::string&);
	uint64_t ToUInt64(const std::string&);
	float ToFloat(const std::string&);

	static inline int32_t FromNumber(char c){
		return c-'0';
	}



	static inline char ToChar(int32_t value){
		return '0'+value;
	}



	bool IsNumberPattern(const std::string&);
	/* S:={'+','-'},N:=[0,9],P:={'.'}
	 * (S|0)N#(PN*|0)
	 */

	bool IsUnsignedNumberPatten(const std::string&);
	/* N=[0,9]
	 * ('+',0)N#
	 */

	bool IsOptionPattern(const std::string&);
	 /* A=['a','Z'],N=[0,9],C={A|N}
	  * '-'C#
	 */

	class CmdParser{
	private:
		static constexpr uint32_t OptionSize = 16;
		std::string cmd;
		std::string option[OptionSize];
		std::string value[OptionSize];
		uint32_t len;
	public:
		CmdParser();
		virtual ~CmdParser() = default;
		bool Parse(text_iterator begin, text_iterator);
		bool Search(const std::string&, uint32_t* index = nullptr);
		void Clear();
		bool IsOptionNull();
		const std::string GetOptionArg(uint32_t index = 0);
	};
	extern CmdParser cmdParser;

} /* namespace common */

#include <text.tpp>
#endif /* COMMON_INC_TEXT_HPP_ */
