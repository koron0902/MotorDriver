#include <File.hpp>
#include <mempool.hpp>
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
static MemPool<FileBase::MaxSize, FileBase::MaxNumber> pool;
Directory *root { nullptr }, *current { nullptr };

void* FileBase::operator new(size_t sz) {
	if (sz > MaxSize) {
		return NULL; //クラスの大きさが不正
	}
	return pool.CreatePointer();
}

void FileBase::operator delete(void* ptr) {
	pool.ReleasePointer(ptr);
}

FileBase::FileBase(const string& _name) {
	name = _name;
}

FileBase::~FileBase() {
	if (next != nullptr) {
		delete next;
		next = nullptr;
	}
	if (child != nullptr) {
		delete child;
		child = nullptr;
	}
}

void FileBase::Add(FileBase* ptr) {
	//これは自身が親になる。
	if (ptr == nullptr)
		return; //無視

	ptr->next = child;
	child = ptr;
	ptr->parent = this;
}

string FileBase::operator()(std::vector<std::string>& s) {
	return "NonSupport(Base)";
}

string FileBase::GetData() {
	return "NonSupport(Base)";
}

string FileBase::SetData(const string& str) {
	return "NonSupport(Base)";
}

FileBase* FileBase::SearchChilren(const string& name) {
	for (auto *it = child; it != nullptr; it = it->next) {
		if (it->name == name) {
			return it;
		}
	}
	return nullptr;
}

FileBase* FileBase::Search(const vector<string>& lst) {
	FileBase* it = this;
	for (const string& cmp : lst) {
		if (cmp == "..") {
			it = it->parent;
		} else if (cmp == ".") {
			//何もしない
		} else {
			it = it->SearchChilren(cmp);
			if (it == nullptr)
				return nullptr;
		}
	}
	return it;
}

FileBase* FileBase::Search(const string& path) {
	return Search(Split(path, "/"));
}

std::string FileBase::GetPathName() const {
	if (parent != nullptr) {
		return parent->GetPathName() + name + '/';
	} else {
		return "/";
	}
}

std::string FileBase::GetChildrenName() const {
	if (child != nullptr)
		return child->GetChildrenNameSub();
	return "";
}

string FileBase::GetAllName(unsigned int sp) const {
	string ans = Space("|", sp) + '+' + name + newline;
	for (auto it = child; it != nullptr; it = it->next) {
		ans += it->GetAllName(sp + 1);
	}
	return ans;
}

size_t FileBase::GetMemorySizeAll() {
	return pool.CountAreaByte();
}
size_t FileBase::GetMemorySizeUsed() {
	return pool.CountUsedByte();
}
size_t FileBase::GetMemorySizeFree() {
	return pool.CountFreeByte();
}

std::string FileBase::GetChildrenNameSub() const {
	if (next != nullptr) {
		return next->GetChildrenNameSub() + newline + name;
	} else {
		return name;
	}
}

Directory::Directory(const string& _name) :
		FileBase(_name) {
	SetFlag(FileType::Directory);
}

Directory* Directory::Create(const string& name) {
	auto *p = new Directory(name);
	return p;
}

void Directory::Add(FileBase* p) {
	FileBase::Add(p);
}

string Directory::operator()(std::vector<std::string>& s) {
	return "Error:(Directory)";
}

Execute::Execute(const string& filename, const command& _func) :
		FileBase(filename) {
	func = _func;
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
		return "set: given value is empty"; //NPE prevention
	if (data != nullptr) {
		*data = ToInt(str);
		return "";
	} else {
		return "null";
	}
}

string FileInt32::operator()(std::vector<std::string>& s) {
	return "Error:(File)";
}

FileFloat::FileFloat(const string& filename, float* f) :
		FileBase(filename) {
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
		if (str.empty())
			return "set: given value is empty"; //NPE prevention
		*data = (float) std::atof(str.data());
	} else {
		return "null";
	}
}

string FileFloat::operator()(std::vector<std::string>& s) {
	return "Error:(File)";
}

FileString::FileString(const string& filename, std::string* str) :
		FileBase(filename) {
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
			return "set: given value is empty"; //NPE prevention
		*data = str;
		return "";
	} else {
		return "null";
	}
}

string FileString::operator()(std::vector<std::string>& s) {
	return "Error:(File)";
}

FileFix::FileFix(const string& filename, fix32* f) :
		FileBase(filename) {
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
			return "set: given value is empty"; //NPE prevention
		*data = ToFix(str);

		return "";
	}else{
		return "null";
	}
}

string FileFix::operator()(std::vector<std::string>& s) {
	return "Error:(File)";
}
}
} /* namespace App */
