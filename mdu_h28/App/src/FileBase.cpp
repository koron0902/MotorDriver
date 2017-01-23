#include "FileBase.hpp"
#include <mempool.hpp>
#include <text.hpp>
#include <XPort.hpp>
#include <algorithm>
using namespace common;
using namespace std;
using namespace Middle;
namespace App {
namespace File {

static MemPool<FileBase::MaxSize, FileBase::MaxNumber> pool;
static LockedPool<FileBase::MaxText> text_pool;
static size_t size=0;
Directory *root { nullptr }, *current { nullptr };

void* FileBase::operator new(size_t sz) {
	if (sz > MaxSize) {
		return NULL; //クラスの大きさが不正
	}
	size=max(sz,size);
	return pool.CreatePointer();
}

void FileBase::operator delete(void* ptr) {
	pool.ReleasePointer(ptr);
}

FileBase::FileBase(const string& _name) :
	name(text_pool.Clone(_name.c_str()))
		{


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

int FileBase::operator()(text_iterator begin, text_iterator end) {
	XPort::WriteLine(
			"AccessError");
	return -1;
}

string FileBase::GetData() {
	return "AccessError";
}

int FileBase::SetData(const string& str) {
	XPort::WriteLine("AccessError");
	return -2;
}

FileBase* FileBase::SearchChilren(const string& name) {
	for (auto *it = child; it != nullptr; it = it->next) {
		if (it->name == name) {
			return it;
		}
	}
	return nullptr;
}

FileBase* FileBase::Search(const text_vector& lst) {
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
	string ans;
	bool flag=false;
	for (auto it=this->child;it!=nullptr;it=it->next){
		if (flag){
			ans+=comma;
		}else{
			flag=true;
		}
		ans+=it->name;
	}
	return ans;
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

size_t FileBase::MaxItemSize(){
	return size;
}

size_t FileBase::GetTextSizeUsed(){
	return text_pool.CountUsedByte();
}

size_t FileBase::GetTextSizeFree(){
	return text_pool.CountFreeByte();
}

size_t FileBase::GetTextSizeAll(){
	return text_pool.CountAllByte();
}

Directory::Directory(const string& _name) :
		FileBase(_name) {
	SetType(FileType::Directory);
}

Directory* Directory::Create(const string& name) {
	auto *p = new Directory(name);
	return p;
}

void Directory::Add(FileBase* p) {
	FileBase::Add(p);
}

}
} /* namespace App */
