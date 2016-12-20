/*
 * Bin.h
 *
 *  Created on: 2016/11/14
 *      Author: hamus
 */

#ifndef APP_BIN_H_
#define APP_BIN_H_

#include <FileBase.hpp>

namespace App {
namespace Bin {
using namespace common;
File::Directory* Create();

using command=int(*)(text_iterator,text_iterator);
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
File::FileBase* CreateRepeat();
File::FileBase* CreateTest();
File::FileBase* CreateReboot();
File::FileBase* CreateDelay();
}
} /* namespace Device */

#endif /* APP_BIN_H_ */
