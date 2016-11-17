#include <Bin.hpp>
#include <File.hpp>
#include <sstream>
#include <Timer.hpp>
#include <text.hpp>
#include <stdio.h>

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
			if (file->GetFlag() != FileType::Directory) return string{
				"cd: not a directory" };
			File::current = (Directory*) file;
			return "";
		} else {
			return string{"cd: directory not found" };
		}
	}
	return "";
}

string echo(const std::vector<std::string>& arg) {
	string str;
	for (unsigned int i = 1; i < arg.size(); i++) {
		str += arg[i];
		if (i < arg.size()) str += " ";
	}
	return str;
}

string get(const std::vector<std::string>& arg) {
	//TODO: get stub
	if (arg.size() == 1) {
		return string{"get: no file selected" };
	} else if (arg[1].empty()) {
		return string{"get: no file selected" };
	}
	auto* file = File::current->Search(arg[1]);
	if (file != nullptr) {
		auto flag = file->GetFlag();
		if (flag != FileType::FileInt32 && flag != FileType::FileFloat
				&& flag != FileType::FileString) return string{"get: not a file" };
		return file->GetData();
	} else {
		return string{"get: file not found" };
	}
}

string set(const std::vector<std::string>& arg) {
	//TODO: set stub
	if (arg.size() == 1) {
		return string{"set: no file selected" };
	} else if (arg[1].empty()) {
		return string{"set: no file selected" };
	} else if (arg.size() == 2) {
		return string{"set: no value entered" };
	} else if (arg[2].empty()) {
		return string{"set: no value entered" };
	}
	auto* file = File::current->Search(arg[1]);
	if (file != nullptr) {
		if (file->GetFlag() != FileType::FileInt32
				&& file->GetFlag() != FileType::FileFloat
				&& file->GetFlag() != FileType::FileString) return string{
			"set: not a file" };
		return file->SetData(arg[2]);
	} else {
		return string{"set: file not found" };
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
	ss += "System info" + newline;
	ss += "File Memory[Byte]:" + (FileBase::GetMemorySizeAll()) + ','
			+ ToStr(FileBase::GetMemorySizeUsed()) + ','
			+ ToStr(FileBase::GetMemorySizeFree()) + newline;
	ss =+ "Stamp:"+ToStr(Device::Timer::GetSystemTime())+newline;
	return ss;
}
}
} /* namespace Device */
