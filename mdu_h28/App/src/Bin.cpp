#include <Bin.hpp>
#include <File.hpp>
#include <sstream>
#include <Timer.hpp>
#include <text.hpp>
#include <stdio.h>
#include <xport.hpp>
#include <Shell.hpp>
#include <SysTick.hpp>
#include <cmsis.h>
#include <core_cm3.h>
#include <iap.h>
#include <ff.hpp>
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
	bin->Add(Execute::Create("mkfs",mkfs));
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
	if (arg.size() == 1) {
		return string { "get: no file selected" };
	} else if (arg[1].empty()) {
		return string { "get: no file selected" };
	}
	auto* file = File::current->Search(arg[1]);
	if (file != nullptr) {
		auto flag = file->GetFlag();
		if (flag != FileType::FileInt32 && flag != FileType::FileFloat
				&& flag != FileType::FileString)
			return string { "get: not a file" };
		return file->GetData();
	} else {
		return string { "get: file not found" };
	}
}

string set(const std::vector<std::string>& arg) {
	//TODO: set stub
	if (arg.size() == 1) {
		return string { "set: no file selected" };
	} else if (arg[1].empty()) {
		return string { "set: no file selected" };
	} else if (arg.size() == 2) {
		return string { "set: no value entered" };
	} else if (arg[2].empty()) {
		return string { "set: no value entered" };
	}
	auto* file = File::current->Search(arg[1]);
	if (file != nullptr) {
		if (file->GetFlag() != FileType::FileInt32
				&& file->GetFlag() != FileType::FileFloat
				&& file->GetFlag() != FileType::FileString)
			return string { "set: not a file" };
		return file->SetData(arg[2]);
	} else {
		return string { "set: file not found" };
	}
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

std::string reboot(const std::vector<std::string>& dummy){
NVIC_SystemReset();
return "";//dummy cannot reach here
}

std::string mkfs(const std::vector<std::string>& dummy){
	uint8_t work[512];
	Chip_IAP_PreSectorForReadWrite(0x3C,0x3F);
	Chip_IAP_EraseSector(0x3C,0x3F);
	Middle::FatFs::f_mkfs("0", (FM_FAT | FM_SFD), 0, work, 512);
	return "";
}
}
} /* namespace Device */
