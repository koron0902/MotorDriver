#include <Mid.hpp>
#include <Motor.hpp>
#include <stdlib.h>
#include <text.hpp>
using namespace Middle;
using namespace App::File;
using namespace common;

namespace App {
namespace Mid{

Directory* Create(){
	auto *mid=Directory::Create("mid");
	mid->Add(Execute::Create("duty",Duty));
	mid->Add(Execute::Create("free",Free));
	mid->Add(Execute::Create("lock",Lock));
	return mid;
}

std::string Duty(const common::ShellParameter& arg){
	if (arg.size()>1){
		Motor::SetDuty(common::ToFix(arg[1]));
	}else{
		Motor::Lock();
	}
	return "";
}
std::string Free(const common::ShellParameter&){
	Motor::Free();
	return "";
}

std::string Lock(const common::ShellParameter&){
	Motor::Lock();
	return "";
}





}

} /* namespace App */
