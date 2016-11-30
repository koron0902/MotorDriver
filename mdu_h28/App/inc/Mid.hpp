#ifndef APP_MID_H_
#define APP_MID_H_

#include <File.hpp>
#include <type.hpp>
#include <vector>
#include <string>
namespace App {
namespace Mid{

using iterator = App::File::iterator;

File::Directory* Create();

}
} /* namespace App */

#endif /* APP_MID_H_ */
