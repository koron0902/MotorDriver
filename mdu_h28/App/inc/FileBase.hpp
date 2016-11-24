#pragma once

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <string>

namespace App {
namespace File {

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

struct FileMode {
	const static FileMode None, Execute, WriteOnly, ReadOnly, WriteAndRead,
			Protect;
private:
	uint8_t flag;
public:
	constexpr FileMode(uint8_t res = 0) :
			flag(res) {
	}
	constexpr FileMode(const FileMode&) = default;

	constexpr FileMode operator |(FileMode m) const {
		return FileMode(flag | m.flag);
	}
	constexpr FileMode operator |=(FileMode m) {
		return FileMode(flag |= m.flag);
	}
	operator int() const {
		return flag;
	}

	FileMode operator =(FileMode m){
		return FileMode(flag=m.flag);
	}

	bool IsExecutable() const {
		return flag & Execute;
	}
	bool IsWritable() const {
		return flag & WriteOnly;
	}
	bool IsReadable() const {
		return flag & ReadOnly;
	}
	bool IsProtected() {
		return flag & Protect;
	}
};

class FileBase {
public:
	static constexpr int MaxNumber = 32; //ファイルの最大数
	static constexpr int MaxSize = 80; //派生先を含むクラスの最大サイズ
private:
	std::string name;
	//本当はスマートポインタを使うはずだった..
	FileBase* parent { nullptr };
	FileBase* child { nullptr };
	FileBase* next { nullptr };
	FileType type { FileType::None };
	FileMode mode{FileMode::None};
protected:
	static void* operator new(size_t);
	static void operator delete(void*);

	void SetFlag(FileType _flag) {
		type = _flag;
	}
	void SetMode(FileMode _mode) {
		mode = _mode;
	}
	void Add(FileBase*); //子要素として追加
public:
	FileBase(const std::string& name); //本来はprotected
	virtual ~FileBase();

	virtual std::string operator()(std::vector<std::string>&);
	FileType GetFlag() const {
		return type;
	}

	FileMode GetMode() const {
		return mode;
	}

	const std::string& GetName() const {
		return name;
	}
	FileBase* SearchChilren(const std::string& name);
	FileBase* Search(const std::vector<std::string>&);
	FileBase* Search(const std::string&);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);
	std::string GetPathName() const;
	std::string GetChildrenName() const;
	std::string GetAllName(unsigned int sp = 0) const;
	//メモリーの配置状態の取得
	static size_t GetMemorySizeAll();
	static size_t GetMemorySizeUsed();
	static size_t GetMemorySizeFree();
private:
	std::string GetChildrenNameSub() const;
};
}
}

