#ifndef APP_MID_H_
#define APP_MID_H_

#include <FileBase.hpp>



namespace App {
namespace Mid{
File::Directory* Create();

File::Directory* CreateDRV();
File::Directory* CreateTrap();
File::Directory* CreatePID();
File::Directory* CreateBasic();

File::FileBase* CreateSwitch();
File::FileBase* CreateDuty();
File::FileBase* CreateFree();
File::FileBase* CreateLock();
File::FileBase* CreateParam();

}
} /* namespace App */

#endif /* APP_MID_H_ */
