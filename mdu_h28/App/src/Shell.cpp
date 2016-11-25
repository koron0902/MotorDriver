#include <Bin.hpp>
#include <Dev.hpp>
#include <Mid.hpp>
#include <text.hpp>
#include "TaskManager.hpp"
#include <Shell.hpp>
#include <Uart.hpp>
#include <xport.hpp>
using namespace std;
using namespace common;
using namespace App::File;

namespace App {

namespace Shell {

static std::vector<std::string> history; //過去の命令をひとつだけ保持する

static std::vector<File::Directory*> path; //省略用

void Init() {
	Directory* bin;Property *p;
	current = root = Directory::Create("root");
	root->Add(bin = Bin::Create());
	root->Add(Dev::Create());
	root->Add(Mid::Create());
	path.push_back(bin);
}

string Call(std::vector<std::string>& arg) {
	if (arg.empty())
		return "";
	history = arg;
	auto& method = arg[0];
	FileBase* file;

	if ((file = current->Search(method)) != nullptr) {
		if (file->GetMode().IsExecutable()) {
			return (*file)(arg);
		} else {
			return "NonExecute";
		}
	}

	for (auto dir : path) {
		if ((file = dir->Search(method)) != nullptr) {
			if (file->GetMode().IsExecutable()) {
				return (*file)(arg);
			} else {
				return "NonExecute";
			}
		}
	}
	return "Not Exist";
}

string Call(const string& text) {
	string buf = "";
	buf.reserve(64);
	bool flag=false;
	auto lst = Split(text, ":");
	for (auto &commmand : lst) {
		auto sp = Split(commmand, " ");
		buf += flag?Call(sp)+":":Call(sp);
	}
	return buf;
}

}
}
