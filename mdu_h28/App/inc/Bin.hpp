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
using iterator=File::iterator;
using command=int(*)(iterator,iterator);
//基本命令
File::FileBase* CreatePWD();
File::FileBase* CreateCD();
File::FileBase* CreateEcho();
File::FileBase* CreateGet();
File::FileBase* CreateSet();
File::FileBase* CreateLs();
File::FileBase* CreateTree();
File::FileBase* CreateInfo();
File::FileBase* CreateStmp();
int stmp(iterator begin,iterator end);

File::FileBase* CreateRepeat();


int reboot(iterator begin,iterator end);

int test(iterator begin,iterator end);//実験用
}
} /* namespace Device */

#endif /* APP_BIN_H_ */
