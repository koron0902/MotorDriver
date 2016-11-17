/*
 * File.h
 *
 *  Created on: 2016/11/06
 *      Author: TERU
 */

#ifndef APP_FILE_H_
#define APP_FILE_H_

#include <vector>
#include <deque>
#include <string>
#include <stdint.h>
#include <functional>

namespace App {
namespace File{
//ファイルの登録
enum class FileType:uint16_t{
	None=0,//エラー用の空集合
	Directory,
	Execute,//通常の実行可能な関数
	FileInt32,
	FileFloat,
	FileString,
};

union FileMode{
	uint8_t flag;
	struct {
		unsigned executable:1;
		unsigned writable:1;
		unsigned readable:1;
		unsigned protection:1;
	};
};

class FileBase {
public:
	static constexpr int MaxNumber = 32; //ファイルの最大数
	static constexpr int MaxSize = 64; //派生先を含むクラスの最大サイズ
private:
	std::string name;
	//本当はスマートポインタを使うはずだった..
	FileBase* parent{nullptr };
	FileBase* child{nullptr };
	FileBase* next{nullptr };
	FileType flag{FileType::None};
	FileMode mode;
protected:
	static void* operator new(size_t);
	static void operator delete(void*);

	void SetFlag(FileType _flag) {
		flag = _flag;
	}
	void Add(FileBase*); //子要素として追加
public:
	FileBase(const std::string& name); //本来はprotected
	virtual ~FileBase();

	virtual std::string operator()(std::vector<std::string>&);
	FileType GetFlag() const {
		return flag;
	}
	const std::string& GetName() const {
		return name;
	}
	FileBase* SearchChilren(const std::string& name) ;
	FileBase* Search(const std::vector<std::string>&);
	FileBase* Search(const std::string&);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);
	std::string GetPathName()const;
	std::string GetChildrenName()const;
	std::string GetAllName(unsigned int sp = 0) const;
	//メモリーの配置状態の取得
	static size_t GetMemorySizeAll();
	static size_t GetMemorySizeUsed();
	static size_t GetMemorySizeFree();
private:
	std::string GetChildrenNameSub()const;
};

class Directory: public FileBase {
private:
	Directory(const std::string& name);
public:

	virtual ~Directory()=default;
	static Directory* Create(const std::string& name);
	void Add(FileBase* p);
	virtual std::string operator()(std::vector<std::string>&);
};

class Execute:public FileBase{
public:
	using command= std::function<std::string(std::vector<std::string>&)>;
private:
	command func;
	Execute(const std::string& name,const command& func=nullptr);
public:
	static Execute* Create(const std::string& filename, const command& cmd=nullptr);
	virtual std::string operator()(std::vector<std::string>& v);


};

class FileInt32:public FileBase{
private:
	int32_t* data;
	FileInt32(const std::string& filename,int32_t* d);
public:
	static FileInt32* Create(const std::string& filename,int32_t* d);
	std::string GetData();
	std::string SetData(const std::string&);
	virtual std::string operator()(std::vector<std::string>&);
};

class FileFloat:public FileBase{
private:
	float* data;
	FileFloat(const std::string& filename,float* f);
public:
	static FileFloat* Create(const std::string& filename,float* f);
	std::string GetData();
	std::string SetData(const std::string&);
	virtual std::string operator()(std::vector<std::string>&);
};

class FileString:public FileBase{
private:
	std::string* data;
	FileString(const std::string& filename,std::string* str);
public:
	static FileString* Create(const std::string& filename,std::string* str);
	std::string GetData();
	std::string SetData(const std::string&);
	virtual std::string operator()(std::vector<std::string>&);
};

extern Directory *root,*current;


} /* namespace App */
}
#endif /* APP_FILE_H_ */
