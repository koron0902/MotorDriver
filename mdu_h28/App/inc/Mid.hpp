#ifndef APP_MID_H_
#define APP_MID_H_

#include <FileBase.hpp>



namespace App {
namespace Mid{



File::Directory* Create();

File::FileBase* CreateDuty();
File::FileBase* CreateFree();
File::FileBase* CreateLock();

}
} /* namespace App */

#endif /* APP_MID_H_ */
