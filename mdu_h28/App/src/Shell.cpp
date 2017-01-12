#include <Bin.hpp>
#include <Dev.hpp>
#include <Mid.hpp>
#include <Shell.hpp>
#include <XPort.hpp>
using namespace std;
using namespace common;
using namespace App::File;
using namespace Middle;

namespace App {

namespace Shell {

static std::vector<std::string> history; //過去の命令をひとつだけ保持する

static std::vector<File::Directory*> path; //省略用

void Init() {
	Directory* bin;

	current = root = Directory::Create("root");
	root->Add(bin = Bin::Create());
	root->Add(Dev::Create());
	root->Add(Mid::Create());
	path.push_back(bin);
}

int Call(text_iterator begin, text_iterator end) {
	if (distance(begin, end) >= 1) {
		FileBase* file = current->Search(*begin);
		if (file != nullptr) {
			return (*file)(begin, end);
		}

		for (auto dir : path) {
			if ((file = dir->Search(*begin)) != nullptr) {
				return (*file)(begin, end);
			}
		}
		return -1;
	} else {
		return -1;
	}
}

int Call(std::vector<std::string>& arg) {
	return Call(arg.begin(), arg.end());
}

int Call(const string& text) {
	string buf = "";
	buf.reserve(64);
	bool flag = false;
	auto lst = Split(text, ":");
	for (auto &commmand : lst) {
		auto sp = Split(commmand, " ");
		if (flag)
			XPort::Write(":");
		Call(sp);
		flag = true;
	}
	return 0;
}

}
}
