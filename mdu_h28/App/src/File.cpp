#include <File.hpp>

#include <stddef.h>
#include <text.hpp>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>

using namespace common;
using namespace std;

namespace App {
namespace File {

Directory *root { nullptr }, *current { nullptr };

Directory::Directory(const string& _name) :
		FileBase(_name) {
	SetMode(FileMode::None);
	SetFlag(FileType::Directory);
}

Directory* Directory::Create(const string& name) {
	auto *p = new Directory(name);
	return p;
}

void Directory::Add(FileBase* p) {
	FileBase::Add(p);
}
/*
 string Directory::operator()(std::vector<std::string>& s) {
 return "Error:(Directory)";
 }
 */
Execute::Execute(const string& filename, const command& _func) :
		FileBase(filename) {
	func = _func;
	SetMode(FileMode::Execute);
	SetFlag(FileType::Execute);
}

Execute* Execute::Create(const string& filename, const command& cmd) {
	return new Execute(filename, cmd);
}

string Execute::operator()(std::vector<std::string>& v) {
	if (func != nullptr) {
		return func(v);
	} else {
		return "Error:(CMD is Nothing)";
	}
}

FileInt32::FileInt32(const string& filename, int32_t* d) :
		FileBase(filename) {
	SetMode(FileMode::WriteAndRead);
	SetFlag(FileType::FileInt32);
	data = d;
}

FileInt32* FileInt32::Create(const string& filename, int32_t* d) {
	return new FileInt32(filename, d);
}

string FileInt32::GetData() {
	if (data != nullptr) {
		return ToStr(*data);
	} else {
		return "null";
	}
}

string FileInt32::SetData(const std::string& str) {
	if (str.empty())
		return "null"; //NPE prevention
	if (data != nullptr) {
		*data = ToInt(str);
		return "";
	} else {
		return "found out";
	}
}
/*
 string FileInt32::operator()(std::vector<std::string>& s) {
 return "Error:(File)";
 }
 */
FileFloat::FileFloat(const string& filename, float* f) :
		FileBase(filename) {
	SetMode(FileMode::WriteAndRead);
	SetFlag(FileType::FileFloat);
	data = f;
}

FileFloat* FileFloat::Create(const string& filename, float* f) {
	return new FileFloat(filename, f);
}

string FileFloat::GetData() {
	if (data != nullptr) {
		stringstream ss;
		int d1 = (*data);
		float f = (*data) * 10000;
		int d2 = abs((int) (f) % 10000);
		ss << d1 << "." << d2;
		return ss.str();
	} else {
		return "null";
	}
}

string FileFloat::SetData(const std::string& str) {
	if (data != nullptr) {
		if (str.empty()) {
			return "null"; //NPE prevention
		} else {
			*data = (float) std::atof(str.data());
			return "";
		}
	} else {
		return "found out";
	}
}
/*
 string FileFloat::operator()(std::vector<std::string>& s) {
 return "Error:(File)";
 }
 */
FileString::FileString(const string& filename, std::string* str) :
		FileBase(filename) {
	SetMode(FileMode::WriteAndRead);
	SetFlag(FileType::FileString);
	data = str;
}

FileString* FileString::Create(const string& filename, std::string* str) {
	return new FileString(filename, str);
}

string FileString::GetData() {
	if (data != nullptr) {
		return *data;
	} else {
		return "null";
	}
}

string FileString::SetData(const std::string& str) {
	if (data != nullptr) {
		if (str.empty())
			return "null"; //NPE prevention
		*data = str;
		return "";
	} else {
		return "found out";
	}
}
/*
 string FileString::operator()(std::vector<std::string>& s) {
 return "Error:(File)";
 }
 */
FileFix::FileFix(const string& filename, fix32* f) :
		FileBase(filename) {
	SetMode(FileMode::WriteAndRead);
	SetFlag(FileType::FileFix);
	data = f;
}

FileFix* FileFix::Create(const string& filename, fix32* f) {
	return new FileFix(filename, f);
}

string FileFix::GetData() {
	if (data != nullptr) {
		return ToStr(*data);
	} else {
		return "null";
	}
}

string FileFix::SetData(const std::string& str) {
	if (data != nullptr) {
		if (str.empty())
			return "null"; //NPE prevention
		*data = ToFix(str);
		return "";
	} else {
		return "found out";
	}
}
/*
 string FileFix::operator()(std::vector<std::string>& s) {
 return "Error:(File)";
 }
 */
FileProperty::FileProperty(const std::string& filename,
		const std::function<std::string(void)>& get,
		const std::function<std::string(const std::string&)>& set) :
		FileBase(filename), fget(get), fset(set) {
	FileMode mode;
	if (get != nullptr) {
		mode |= FileMode::ReadOnly;
	}
	if (set != nullptr) {
		mode |= FileMode::WriteOnly;
	}
	SetMode(mode);
}

FileProperty* FileProperty::Create(const string& filename,
		const function<string(void)>& get,
		const function<string(const string&)>& set) {
	return new FileProperty(filename, get, set);
}

FileProperty* FileProperty::CreateReadOnly(const std::string& filename,
		const std::function<std::string(void)>& get) {
	return new FileProperty(filename, get, nullptr);
}

FileProperty* FileProperty::CreateWriteOnly(const std::string& filename,
		const std::function<std::string(const std::string&)>& set) {
	return new FileProperty(filename, nullptr, set);
}

string FileProperty::GetData() {
	if (fget != nullptr) {
		return fget();
	} else {
		return "null";
	}
}

string FileProperty::SetData(const string& data) {
	if (fset != nullptr) {
		return fset(data);
	} else {
		return "null";
	}
}

}
} /* namespace App */
