#include <Bin.hpp>
#include <text.hpp>
#include <stdio.h>
#include <xport.hpp>
#include <Shell.hpp>
#include <SysTick.hpp>
#include <cmsis.h>
#include <core_cm3.h>
#include <iap.h>
#include <ff.hpp>
#include <Port.hpp>
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
	bin->Add(CreateTest());
	bin->Add(CreateReboot());
	bin->Add(CreateDelay());
	bin->Add(CreateMkfs());
	return bin;
}

FileBase* CreatePWD() {
	auto pwd = [](text_iterator begin, text_iterator end)->int {
		XPort::WriteLine(current->GetPathName());
		return 0;
	};
	return Execute<decltype(pwd)>::Create("pwd", pwd);
}

FileBase* CreateCD() {
	auto cd = [](text_iterator begin, text_iterator end) {
		text_iterator path = begin + 1;
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
	auto echo = [](text_iterator begin, text_iterator end) {
		begin++; //一つ目には必ず呼び出し元が入っているので省略
			if (begin!=end) {
				XPort::Write(*(begin++));
			} else {
				return 0;
			}
			while (begin!=end) {
				XPort::Write(comma);
				XPort::Write(*(begin++));
			}
			XPort::WriteLine();
			XPort::Flush();
			return 0;
		};
	return Execute<decltype(echo)>::Create("echo", echo);
}

File::FileBase* CreateGet() {
	return File::CreateExecute("get",
			[](text_iterator begin, text_iterator end)->int {
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
	return File::CreateExecute("set",
			[](text_iterator begin, text_iterator end) ->int {
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
	return File::CreateExecute("ls",
			[](text_iterator begin, text_iterator end) {
				XPort::WriteLine(current->GetChildrenName());
				return 0;
			});
}

File::FileBase* CreateTree() {
	return File::CreateExecute("tree",
			[](text_iterator begin, text_iterator end) {
				XPort::WriteLine(current->GetAllName());
				return 0;
			});

}

File::FileBase* CreateInfo() {
	return File::CreateExecute("info",
			[](text_iterator begin, text_iterator end)-> int {

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
		virtual int operator()(text_iterator begin, text_iterator end) {
			if (++begin != end) {
				if (IsOptionPattern(*begin)) {
					if (*begin == "-s") {
						XPort::WriteLine(ToStr(Device::Tick::TickS()));
					} else if (*begin == "-ms") {
						XPort::WriteLine(ToStr(Device::Tick::TickMs()));
					} else if (*begin == "-us") {
						XPort::WriteLine(ToStr(Device::Tick::TickUs()));
					} else if (*begin == "-t") {
						XPort::WriteLine(ToStr(Device::Tick::Tick()));
					} else {
						XPort::WriteLine("NonSurport");
					}
				} else {
					XPort::WriteLine(ToStr(Device::Tick::TickUs()));
				}
			}else{
				XPort::WriteLine(ToStr(Device::Tick::TickUs()));
			}
			return 0;
		}
		virtual string GetData() {
			return ToStr(Device::Tick::TickUs());
		}
		static stmp* Create() {
			return new stmp;
		}

	};
	return stmp::Create();
}

File::FileBase* CreateRepeat() {
	return File::CreateExecute("repeat",
			[](text_iterator begin, text_iterator end) {
				//この関数は本来、stringに返すことをバッファーがオーバーフローすることを防ぐためXPortで出力する。
				//一つ目を飛ばして再作成
				bool flush=false;
				if (++begin!=end) {
					if (IsOptionPattern(*begin)) {
						if (*begin=="-f") {
							flush=true;
						} else if (*begin=="-help") {
							XPort::WriteLine("[option] command");
							XPort::WriteLine("option..-f(flush),-help");
							return 0;
						} else {
							XPort::WriteLine("NonSurport");
							return -2;
						}

						if (++begin==end) {
							return -1;
						}
					}

					while (XPort::IsEmpty()) {
						Shell::Call(begin, end);
						if (flush) {
							XPort::Flush();
						}
					}

					XPort::Flush();
					return 0;
				} else {
					XPort::WriteLine("Empty");
					return -1;
				}
			});

}

File::FileBase* CreateTest() {
	return File::CreateExecute("test",
			[](text_iterator begin, text_iterator end)->int {
				//auto a = fix32::CreateFloat(2.75f);
				XPort::WriteLine("U:" + common::ToStr(Device::Port::HoleU.Get()) +
								" V:" + common::ToStr(Device::Port::HoleV.Get()) +
								" W:" + common::ToStr(Device::Port::HoleW.Get())
				);
				//XPort::WriteLine(ToStr(a));
				return 0;
			});
}

File::FileBase* CreateReboot() {
	return File::CreateExecute("reboot",
			[](text_iterator begin, text_iterator end)->int {
				NVIC_SystemReset();
				return 0; //dummy cannot reach here
			});
}

File::FileBase* CreateDelay() {
	return File::CreateExecute("delay",
			[](text_iterator begin, text_iterator end)->int {
				uint64_t time=10; //[us] default
				if (++begin!=end) {
					if (IsUnsignedNumberPatten(*begin)) {
						time= ToUInt64(*begin);
						if (++begin==end) {
							XPort::WriteLine("Empty");
							return -1;
						}
					}

					if (IsOptionPattern(*begin)) {
						if (*begin=="-us") {
						} else if (*begin=="-ms") {
							time*=1000;
						} else if (*begin=="-s") {
							time*=1000000;
						} else if (*begin=="-help") {
							XPort::WriteLine("[time] [option] command");
							XPort::WriteLine("time..0~");
							XPort::WriteLine("option{-us,-ms,-s,-help}");
							return 0;
						} else {
							XPort::WriteLine("NonSurport");
						}

						if (++begin==end) {
							XPort::WriteLine("Empty");
							return -1;
						}
					}
					Device::Tick::DelayUs(time);

					Shell::Call(begin,end);

					return 0;
				} else {
					XPort::WriteLine("Empty");
					return -1;
				}

			});
}

File::FileBase* CreateMkfs(){
	return File::CreateExecute("mkfs", [](text_iterator, text_iterator)->auto{
		uint8_t work[1024];
		Chip_IAP_PreSectorForReadWrite(0x3C,0x3F);
		Chip_IAP_EraseSector(0x3C,0x3F);
		Middle::FatFs::f_mkfs("0", (FM_FAT | FM_SFD), 0, work, 1024);
		return 0;
	});
}

}
} /* namespace Device */
