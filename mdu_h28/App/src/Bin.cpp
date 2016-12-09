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
using namespace Middle;

namespace App {
namespace Bin {

Directory* Create() {
	Directory *bin = Directory::Create("bin");
	bin->Add(CreatePWD());
	bin->Add(CreateCD());
	bin->Add(CreateEcho());
	bin->Add(CreateGet());
	bin->Add(CreateSet());
	bin->Add(CreateLs());
	bin->Add(CreateTree());
	bin->Add(CreateInfo());
	bin->Add(CreateStmp());
	bin->Add(CreateRepeat());
	bin->Add(Execute<command>::Create("reboot", reboot));

	bin->Add(Execute<command>::Create("test", test));
	return bin;
}

FileBase* CreatePWD() {
	auto pwd = [](iterator begin, iterator end)->int {
		XPort::WriteLine(current->GetPathName());
		return 0;
	};
	return Execute<decltype(pwd)>::Create("pwd", pwd);
}

FileBase* CreateCD() {
	auto cd = [](iterator begin, iterator end) {
		iterator path = begin + 1;
		if (std::distance(begin, end) <= 1) {
			File::current = File::root;
			return 0;
		} else if ((*path) == "") {
			File::current = File::root;
			return -1;
		} else {
			auto* file = File::current->Search(*path);
			if (file != nullptr) {
				if (file->GetFlag() != FileType::Directory) {
					XPort::WriteLine("cd: not a directory");
					return -1;
				} else {
					File::current = (Directory*) file;
					return 0;
				}
			} else {
				XPort::WriteLine("cd: directory not found");
				return -1;
			}
		}
	};
	return Execute<decltype(cd)>::Create("cd", cd);
}

File::FileBase* CreateEcho() {
	auto echo = [](iterator begin, iterator end) {

		if (distance(begin, end) <= 1) {
			for (auto it = begin + 1; it != end; it++) {
				XPort::Write(*it);
			}
			return 0;
		} else {
			return -1;
		}
	};
	return Execute<decltype(echo)>::Create("echo", echo);
}

File::FileBase* CreateGet() {
	return File::CreateExecute("get", [](iterator begin, iterator end)->int {
		if (distance(begin, end) >= 1) {
			string text;
			bool flag = false;
			begin++;
			while (distance(begin, end) >= 1) {
				auto* file = File::current->Search(*begin);
				if (file != nullptr) {
					XPort::Write(
							flag ? "," + file->GetData() : file->GetData());
					flag = true;
				} else {
					XPort::WriteLine("Found Out");
					return -1;
				}
				begin += 1;
			}
			XPort::WriteLine();
			return 0;
		} else {
			XPort::WriteLine("Error:Empty");
			return -1;
		}
	});

}

File::FileBase* CreateSet() {
	return File::CreateExecute("set", [](iterator begin, iterator end) ->int {
		if (distance(begin, end) >= 1) {
			begin++;
			while (distance(begin, end) >= 2) {
				auto* file = File::current->Search(*(begin));
				if (file != nullptr) {
					file->SetData(*(begin+1));
				} else {
					XPort::WriteLine("found out");
					return -1;
				}
				begin+=2;
			}
			return 0;
		} else {
			XPort::WriteLine("Error:Empty");
			return -1;
		}
	});
}
File::FileBase* CreateLs() {
	return File::CreateExecute("ls", [](iterator begin, iterator end) {
		XPort::WriteLine(current->GetChildrenName());
		return 0;
	});
}

File::FileBase* CreateTree() {
	return File::CreateExecute("tree", [](iterator begin, iterator end) {
		XPort::WriteLine(current->GetAllName());
		return 0;
	});

}

File::FileBase* CreateInfo() {
	return File::CreateExecute("info", [](iterator begin, iterator end)-> int {

		uint32_t mem = (FileBase::GetMemorySizeUsed() * 100)
		/ FileBase::GetMemorySizeAll();
		XPort::WriteLine(string("File Memory[%]:") + ToStr(mem));

		uint32_t text = (FileBase::GetTextSizeUsed() * 100)
		/ FileBase::GetTextSizeAll();
		XPort::WriteLine(string("Text Memory[%]:") + ToStr(text));

		XPort::WriteLine("File Size@Item:" + ToStr(FileBase::MaxItemSize()));
		XPort::WriteLine("TimeStamp:" + ToStr(Device::Tick::TickUs()));
		return 0;
	});
}

File::FileBase* CreateStmp() {
	//実は最も高速な方法
	class stmp: public FileBase {
	public:
		stmp() :
				FileBase("stmp") {
				SetType(FileType::Execute);
		}
		virtual int operator()(iterator begin, iterator end) {
			uint64_t temp = Device::Tick::TickUs();
			XPort::WriteLine(ToStr(temp));
			return 0;
		}
		virtual string GetData(){
			return ToStr(Device::Tick::TickUs());
		}



		static stmp* Create(){
			return new stmp;
		}

	};
	return stmp::Create();
}

File::FileBase* CreateRepeat() {
return File::CreateExecute("repeat", [](iterator begin, iterator end) {
//この関数は本来、stringに返すことをバッファーがオーバーフローすることを防ぐためXPortで出力する。
		using namespace Device::Tick;
		uint64_t w;
//一つ目を飛ばして再作成
		XPort::WriteLine("!finish before pressing any keys");
		begin++;
		while (XPort::IsEmpty()) {
			Shell::Call(begin, end);
			XPort::Flush();
			w = Tick() + 0xFFF;
			while (Tick() < w)
			;
		}
		XPort::WriteLine("!Fin");
		return 0;
	});

}
int test(iterator begin, iterator end) {
auto a = fix32::CreateFloat(2.75f);
XPort::WriteLine(ToStr(a));
return 0;
}

int reboot(iterator begin, iterator end) {
NVIC_SystemReset();
return 0; //dummy cannot reach here
}

}
} /* namespace Device */
