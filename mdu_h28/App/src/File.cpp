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
/*
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
*/
Integer::Integer(const string& filename, int32_t* d) :
		FileBase(filename) {
	SetMode(FileMode::WriteAndRead);
	SetFlag(FileType::FileInt32);
	data = d;
}

Integer* Integer::Create(const string& filename, int32_t* d) {
	return new Integer(filename, d);
}

string Integer::GetData() {
	if (data != nullptr) {
		return ToStr(*data);
	} else {
		return "null";
	}
}

string Integer::SetData(const std::string& str) {
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
Float::Float(const string& filename, float* f) :
		FileBase(filename) {
	SetMode(FileMode::WriteAndRead);
	SetFlag(FileType::FileFloat);
	data = f;
}

Float* Float::Create(const string& filename, float* f) {
	return new Float(filename, f);
}

string Float::GetData() {
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

string Float::SetData(const std::string& str) {
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
String::String(const string& filename, std::string* str) :
		FileBase(filename) {
	SetMode(FileMode::WriteAndRead);
	SetFlag(FileType::FileString);
	data = str;
}

String* String::Create(const string& filename, std::string* str) {
	return new String(filename, str);
}

string String::GetData() {
	if (data != nullptr) {
		return *data;
	} else {
		return "null";
	}
}

string String::SetData(const std::string& str) {
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
Fix::Fix(const string& filename, fix32* f) :
		FileBase(filename) {
	SetMode(FileMode::WriteAndRead);
	SetFlag(FileType::FileFix);
	data = f;
}

Fix* Fix::Create(const string& filename, fix32* f) {
	return new Fix(filename, f);
}

string Fix::GetData() {
	if (data != nullptr) {
		return ToStr(*data);
	} else {
		return "null";
	}
}

string Fix::SetData(const std::string& str) {
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
Property::Property(const std::string& filename,
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

Property* Property::Create(const string& filename,
		const function<string(void)>& get,
		const function<string(const string&)>& set) {
	return new Property(filename, get, set);
}

Property* Property::CreateReadOnly(const std::string& filename,
		const std::function<std::string(void)>& get) {
	return new Property(filename, get, nullptr);
}

Property* Property::CreateWriteOnly(const std::string& filename,
		const std::function<std::string(const std::string&)>& set) {
	return new Property(filename, nullptr, set);
}

string Property::GetData() {
	if (fget != nullptr) {
		return fget();
	} else {
		return "null";
	}
}

string Property::SetData(const string& data) {
	if (fset != nullptr) {
		return fset(data);
	} else {
		return "null";
	}
}

}
} /* namespace App */
