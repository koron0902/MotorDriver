/*
 * File.h
 *
 *  Created on: 2016/11/06
 *      Author: TERU
 */

#ifndef APP_FILE_H_
#define APP_FILE_H_

#include <functional>
#include <fix.hpp>
#include <FileBase.hpp>
namespace App {
namespace File {
using namespace common;

class Directory: public FileBase {
private:
	Directory(const std::string& name);
public:
	virtual ~Directory() = default;
	static Directory* Create(const std::string& name);
	void Add(FileBase* p);
	//virtual std::string operator()(std::vector<std::string>&);
};

class Execute: public FileBase {
public:
	using command= std::function<std::string(std::vector<std::string>&)>;
private:
	command func;
	Execute(const std::string& name, const command& func = nullptr);
public:
	static Execute* Create(const std::string& filename, const command& cmd =
			nullptr);
	virtual std::string operator()(std::vector<std::string>& v);

};

class FileInt32: public FileBase {
private:
	int32_t* data;
	FileInt32(const std::string& filename, int32_t* d);
public:
	static FileInt32* Create(const std::string& filename, int32_t* d = nullptr);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);
	//virtual std::string operator()(std::vector<std::string>&);
};

class FileFloat: public FileBase {
private:
	float* data;
	FileFloat(const std::string& filename, float* f);
public:
	static FileFloat* Create(const std::string& filename, float* f = nullptr);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);
	//virtual std::string operator()(std::vector<std::string>&);
};

class FileString: public FileBase {
private:
	std::string* data;
	FileString(const std::string& filename, std::string* str = nullptr);
public:
	static FileString* Create(const std::string& filename, std::string* str);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);
	//virtual std::string operator()(std::vector<std::string>&);
};

class FileFix: public FileBase {
private:
	fix32* data;
	FileFix(const std::string& filename, fix32* f = nullptr);
public:
	static FileFix* Create(const std::string& filename, fix32* f);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);
	//virtual std::string operator()(std::vector<std::string>&);
};

class FileProperty: public FileBase {
private:
	std::function<std::string(void)> fget;
	std::function<std::string(const std::string&)> fset;
	FileProperty(const std::string& filename,
			const std::function<std::string(void)>& get,
			const std::function<std::string(const std::string&)>& set);
public:
	static FileProperty* Create(const std::string& filename,
			const std::function<std::string(void)>& get,
			const std::function<std::string(const std::string&)>& set);
	static FileProperty* CreateReadOnly(const std::string& filename,
			const std::function<std::string(void)>& get);
	static FileProperty* CreateWriteOnly(const std::string& filename,
			const std::function<std::string(const std::string&)>& set);
	 virtual std::string GetData();
	 virtual std::string SetData(const std::string&);

};

extern Directory *root, *current;

} /* namespace App */
}
#endif /* APP_FILE_H_ */
