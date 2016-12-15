#include <Mid.hpp>
#include <Motor.hpp>
#include <text.hpp>
#include <type.hpp>
#include<File.hpp>
using namespace Middle;
using namespace App::File;
using namespace common;

namespace App {
namespace Mid{
Directory* Create(){
	auto *mid=Directory::Create("mid");
	mid->Add(CreateDuty());
	mid->Add(CreateFree());
	mid->Add(CreateLock());
	return mid;
}

File::FileBase* CreateDuty(){
	return CreateExecute("duty",[](text_iterator begin,text_iterator end)->int{
		if (std::distance(begin,end)>=2){
			begin++;
			Motor::SetDuty(common::ToFix(*begin));
		}else{
			Motor::Lock();
		}
		return 0;
	});
}
File::FileBase* CreateFree(){
	return CreateExecute("free",[](text_iterator being,text_iterator end)->int{
		Motor::Free();
		return 0;
	});
}

File::FileBase* CreateLock(){
	return CreateExecute("lock",[](text_iterator being,text_iterator end)->int{
		Motor::Lock();
		return 0;
	});
}


}

} /* namespace App */
