#include <File.hpp>

#include <stddef.h>
#include <text.hpp>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <XPort.hpp>

using namespace common;
using namespace std;
using namespace Middle;

namespace App {
namespace File {

Integer::Integer(const string& filename, int32_t* d) :
		FileBase(filename) {
	SetType(FileType::FileInt32);
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

int Integer::SetData(const std::string& str) {
	if (str.empty()) {
		XPort::WriteLine("Null");
		return -1; //NPE prevention
	} else if (data != nullptr) {
		*data = ToInt(str);
		return 0;
	} else {
		XPort::WriteLine("found out");
		return -1;
	}
}
/*
 string FileInt32::operator()(std::vector<std::string>& s) {
 return "Error:(File)";
 }
 */
Float::Float(const string& filename, float* f) :
		FileBase(filename) {
	SetType(FileType::FileFloat);
	data = f;
}

Float* Float::Create(const string& filename, float* f) {
	return new Float(filename, f);
}

string Float::GetData() {
	if (data != nullptr) {
		return ToStrF(*data);
	} else {
		return "null";
	}
}

int Float::SetData(const std::string& str) {
	if (data != nullptr) {
		if (str.empty()) {
			XPort::WriteLine("Empty");
			return -1; //NPE prevention
		} else {
			*data =ToFloat(str);
			return 0;
		}
	} else {
		XPort::WriteLine("Null");
		return -1;
	}
}
/*
 string FileFloat::operator()(std::vector<std::string>& s) {
 return "Error:(File)";
 }
 */
String::String(const string& filename, std::string* str) :
		FileBase(filename) {

	SetType(FileType::FileString);
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

int String::SetData(const std::string& str) {
	if (data != nullptr) {
		*data = str;
		return 0;
	} else {
		XPort::WriteLine("Null");
		return -1;
	}
}
/*
 string FileString::operator()(std::vector<std::string>& s) {
 return "Error:(File)";
 }
 */
Fix::Fix(const string& filename, fix32* f) :
		FileBase(filename) {
	SetType(FileType::FileFix);
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

int Fix::SetData(const std::string& str) {
	if (data != nullptr) {
		if (str.empty()) {
			XPort::WriteLine("Null");
			return -5; //NPE prevention
		}
		*data = ToFix(str);
		return 0;
	} else {
		XPort::WriteLine("FoundOut");
		return -3;
	}
}

}
} /* namespace App */
