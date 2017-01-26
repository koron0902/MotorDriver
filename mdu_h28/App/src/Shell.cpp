#include <Bin.hpp>
#include <Dev.hpp>
#include <EntryMiddle.hpp>
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

static char line_discard = '#'; //この文字が末尾にあった場合、その行を無視

void Init() {
	Directory* bin;

	current = root = Directory::Create("root");
	root->Add(bin = Bin::Create());
	root->Add(Dev::Create());
	root->Add(Middle::Create());
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
		XPort::WriteLine("Missing");
		return -1;
	} else {
		return -1;
	}
}

int Call(std::vector<std::string>& arg) {
	return Call(arg.begin(), arg.end());
}

int Call(const string& text) {
	bool flag = false;

	if (!text.empty()) {
		//末尾を確認
		if (text[text.length() - 1] != line_discard) {
			auto lst = Split(text, ":");
			for (auto &commmand : lst) {
				auto sp = Split(commmand, " ");
				if (flag) XPort::Write(":");
				int response = Call(sp);
				if (response < 0) {
					return response;
				}
				flag = true;
			}
			return 0;
		}else{
			//この行は破棄された
			return 0;
		}
	} else {
		return 0;
	}

}

}
}
