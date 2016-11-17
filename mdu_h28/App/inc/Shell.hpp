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
	std::string Call(std::vector<std::string>&);
	std::string Call(const std::string&);


}
}


#endif /* APP_SHELL_H_ */
