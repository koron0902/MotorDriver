#include <Bin.hpp>
#include <Dev.hpp>
#include <text.hpp>
#include "TaskManager.hpp"
#include <Shell.hpp>
#include <Uart.hpp>
#include <Mid.h>
using namespace std;
using namespace common;
using namespace App::File;

namespace App {

namespace Shell {




static std::vector<std::string> history; //過去の命令をひとつだけ保持する

static std::vector<File::Directory*> path;//省略用

void Init() {
	Directory* bin;
	current = root = Directory::Create("root");
	root->Add(bin=Bin::Create());
	root->Add(CreateDev());
	root->Add(Mid::Create());
	path.push_back(bin);
}

string Call(std::vector<std::string>& arg) {
	if (arg.empty()) return "";
	history = arg;
	auto& method=arg[0];
	FileBase* file;

	if ((file=current->Search(method))!=nullptr){
		return (*file)(arg);
	}

	for (auto dir:path){
		if ((file=dir->Search(method))!=nullptr){
			return (*file)(arg);
		}
	}
	return "Not Exist";
}

string Call(const string& text) {
	auto v = Split(text, " ");
	return Call(v);
}

}
}
