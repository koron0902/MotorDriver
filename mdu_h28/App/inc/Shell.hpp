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
	//Initialize
	void Init();
	//shellの呼び出し
	using iterator=File::iterator;
	int Call(iterator begin,iterator end);
	int Call(const std::vector<std::string>&);
	int Call(const std::string&);
}
}


#endif /* APP_SHELL_H_ */
