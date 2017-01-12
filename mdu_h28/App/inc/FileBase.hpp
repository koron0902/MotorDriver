#pragma once

#include <type.hpp>
namespace App {
namespace File {
using namespace common;
using execute_fp = int(*)(text_iterator,text_iterator);
using set_fp=int(*)(const std::string&);
using get_fp=std::string(*)(void);

enum class FileType
	:uint16_t {
		None = 0, //エラー用の空集合
	Directory,
	Execute, //通常の実行可能な関数
	Properties, //動的に値を取得するもの、与えるもの
	FileInt32,
	FileFloat,
	FileString,
	FileFix,
};

class FileBase {
public:
	static constexpr uint32_t MaxNumber = 64; //ファイルの最大数
	static constexpr size_t MaxSize = 32; //派生先を含むクラスの最大サイズ[Byte]
	static constexpr size_t MaxText=256;//ファイルシステムで使う文字列の合計数[Byte]
private:
	//std::string name;
	const char* name;
	//本当はスマートポインタを使うはずだった..
	FileBase* parent { nullptr };
	FileBase* child { nullptr };
	FileBase* next { nullptr };
	FileType type { FileType::None };
protected:
	static void* operator new(size_t);
	static void operator delete(void*);
	void SetType(FileType _flag) {
		type = _flag;
	}

	void Add(FileBase*); //子要素として追加
public:
	FileBase(const std::string& name); //本来はprotected
	virtual ~FileBase();

	//virtual std::string operator()(std::vector<std::string>&);
	virtual int operator ()(text_iterator begin,text_iterator end);
	FileType GetFlag() const {
		return type;
	}

	std::string GetName() const {
		return name;
	}
	FileBase* SearchChilren(const std::string& name);
	FileBase* Search(const text_vector&);
	FileBase* Search(const std::string&);
	virtual std::string GetData();
	virtual int SetData(const std::string&);
	std::string GetPathName() const;
	std::string GetChildrenName() const;
	std::string GetAllName(unsigned int sp = 0) const;
	//メモリーの配置状態の取得
	static size_t GetMemorySizeAll();
	static size_t GetMemorySizeUsed();
	static size_t GetMemorySizeFree();
	static size_t MaxItemSize();
	static size_t GetTextSizeUsed();
	static size_t GetTextSizeFree();
	static size_t GetTextSizeAll();
private:
	std::string GetChildrenNameSub() const;
};

class Directory: public FileBase {
private:
	Directory(const std::string& name);
public:
	virtual ~Directory() = default;
	static Directory* Create(const std::string& name);
	void Add(FileBase* p);
	//virtual std::string operator()(std::vector<std::string>&);
};


extern Directory *root, *current;

}
}

