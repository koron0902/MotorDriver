/*
 * Shell.h
 *
 *  Created on: 2016/11/06
 *      Author: TERU
 */

#ifndef APP_SHELL_H_
#define APP_SHELL_H_

#include <File.hpp>

namespace App {
namespace Shell{
using namespace common;
	//Initialize
	void Init();
	//shellの呼び出し
	int Call(text_iterator begin,text_iterator end);
	int Call(const text_vector&);
	int Call(const std::string&);
}
}


#endif /* APP_SHELL_H_ */
