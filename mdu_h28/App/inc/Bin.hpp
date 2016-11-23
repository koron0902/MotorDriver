/*
 * Bin.h
 *
 *  Created on: 2016/11/14
 *      Author: hamus
 */

#ifndef APP_BIN_H_
#define APP_BIN_H_

#include <File.hpp>

namespace App {
namespace Bin {

File::Directory* Create();

//基本命令
std::string pwd(const std::vector<std::string>&);
std::string cd(const std::vector<std::string>&);
std::string echo(const std::vector<std::string>&);
std::string get(const std::vector<std::string>&);
std::string set(const std::vector<std::string>&);
std::string ls(const std::vector<std::string>&);
std::string tree(const std::vector<std::string>&);

std::string info(const std::vector<std::string>&);//システムの状態を表示する
std::string stmp(const std::vector<std::string>&);
std::string repeat(const std::vector<std::string>&);

std::string reboot(const std::vector<std::string>&);
}
} /* namespace Device */

#endif /* APP_BIN_H_ */
