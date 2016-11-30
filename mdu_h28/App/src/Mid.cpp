#include <Mid.hpp>
#include <Motor.hpp>
#include <stdlib.h>
#include <text.hpp>
#include <iterator>
#include <vector>
#include <string>
using namespace Middle;
using namespace App::File;
using namespace common;

namespace App {
namespace Mid{

auto Duty=[](iterator begin,iterator end)->int{
	if (std::distance(begin,end)>=2){
		begin++;
		Motor::SetDuty(common::ToFix(*begin));
	}else{
		Motor::Lock();
	}
	return 0;
};
auto Free=[](iterator being,iterator end)->int{
	Motor::Free();
	return 0;
};

auto Lock=[](iterator being,iterator end)->int{
	Motor::Lock();
	return 0;
};

Directory* Create(){
	auto *mid=Directory::Create("mid");
	mid->Add(Execute<decltype(Duty)>::Create("duty",Duty));
	mid->Add(Execute<decltype(Free)>::Create("free",Free));
	mid->Add(Execute<decltype(Lock)>::Create("lock",Lock));
	return mid;
}

}

} /* namespace App */
