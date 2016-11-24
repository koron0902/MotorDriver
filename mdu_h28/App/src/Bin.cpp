#include <Bin.hpp>
#include <File.hpp>
#include <sstream>
#include <Timer.hpp>
#include <text.hpp>
#include <stdio.h>
#include <Shell.hpp>
#include <SysTick.hpp>
#include <cmsis.h>
#include <core_cm3.h>
#include <fix.hpp>
#include <XPort.hpp>
using namespace std;
using namespace App::File;
using namespace common;

namespace App {
namespace Bin {

Directory* Create() {
	Directory *bin = Directory::Create("bin");
	bin->Add(Execute::Create("pwd", pwd));
	bin->Add(Execute::Create("cd", cd));
	bin->Add(Execute::Create("echo", echo));
	bin->Add(Execute::Create("get", get));
	bin->Add(Execute::Create("set", set));
	bin->Add(Execute::Create("ls", ls));
	bin->Add(Execute::Create("tree", tree));
	bin->Add(Execute::Create("info", info));
	bin->Add(Execute::Create("stmp", stmp));
	bin->Add(Execute::Create("repeat", repeat));
	bin->Add(Execute::Create("reboot",reboot));


	bin->Add(Execute::Create("test",test));
	return bin;
}

string pwd(const std::vector<std::string>& dummy) {
	return current->GetPathName();
}

string cd(const std::vector<std::string>& arg) {
	if (arg.size() <= 1) {
		File::current = File::root;
		return "";
	} else if (arg[1].empty()) {
		File::current = File::root;

		return "";
	} else {
		auto* file = File::current->Search(arg[1]);
		if (file != nullptr) {
			if (file->GetFlag() != FileType::Directory)
				return string { "cd: not a directory" };
			File::current = (Directory*) file;
			return "";
		} else {
			return string { "cd: directory not found" };
		}
	}
	return "";
}

string echo(const std::vector<std::string>& arg) {
	string str;
	for (unsigned int i = 1; i < arg.size(); i++) {
		str += arg[i];
		if (i < arg.size())
			str += " ";
	}
	return str;
}

string get(const std::vector<std::string>& arg) {
	//TODO: get stub
	int idx;
	int length=arg.size();
	string text;
	bool flag=false;
	text.reserve(64);
	for (idx=1;idx<length;idx++){
		auto* file = File::current->Search(arg[idx]);
		if (file!=nullptr){
			auto mode=file->GetMode();
			if (mode.IsReadable()){
				text+=flag?","+file->GetData():file->GetData();
				flag=true;
			}else{
				return "Access Error";
			}
		}else{
			return "found out";
		}
	}

	return text;

}

string set(const std::vector<std::string>& arg) {
	//TODO: set stub
	int idx;
	int lenght=arg.size();
	for (idx=1;idx+1<lenght;idx+=2){
		auto* file = File::current->Search(arg[idx]);
		if (file!=nullptr){
			auto mode=file->GetMode();
			if (mode.IsWritable()){
				file->SetData(arg[idx+1]);
			}else{
				return "Access Error";
			}
		}else{
			return "found out";
		}
	}
	return "";
}

std::string ls(const std::vector<std::string>& dummy) {
	return current->GetChildrenName();
}

std::string tree(const std::vector<std::string>& dummy) {
	return current->GetAllName();
}

std::string info(const std::vector<std::string>& dummy) {
	string ss;
	ss.reserve(64);
	ss += "System info" + newline;
	ss += "File Memory[Byte]:" + (FileBase::GetMemorySizeAll()) + ','
			+ ToStr(FileBase::GetMemorySizeUsed()) + ','
			+ ToStr(FileBase::GetMemorySizeFree()) + newline;
	ss = +"Stamp:" + ToStr(Device::Tick::Tick()) + newline;
	return ss;
}

std::string stmp(const std::vector<std::string>& dummy){
	uint64_t temp=Device::Tick::Tick();
	return ToStr(temp);
}

std::string repeat(const std::vector<std::string>& vec) {
	//この関数は本来、stringに返すことをバッファーがオーバーフローすることを防ぐためXPortで出力する。
	using namespace Device::Tick;
	using namespace Middle::XPort;
	std::vector<std::string> sub;
	uint64_t w;
	//一つ目を飛ばして再作成
	for (unsigned int idx = 1; idx < vec.size(); idx++) {
		sub.push_back(vec[idx]);
	}

	WriteLine("!finish before pressing any keys");
	while (IsEmpty()) {
		WriteLine(Shell::Call(sub));
		Flush();
		w=Tick()+0xFFF;
		while  (Tick()<w);
	}
	return "!Fin";
}

std::string test(const std::vector<std::string>& dummy){
	auto a=fix32::CreateFloat(2.75f);
	return ToStr(a);
}


std::string reboot(const std::vector<std::string>& dummy){
NVIC_SystemReset();
return "";//dummy cannot reach here
}



}
} /* namespace Device */
