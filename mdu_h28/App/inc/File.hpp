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

template<class Func> //インライン展開しやすいように変更
class Execute: public FileBase {
private:
	Func m_func;
	Execute(const std::string& name, const Func& func) :
			FileBase(name), m_func(func) {
	}
public:
	static Execute<Func>* Create(const std::string& filename, const Func& func) {
		return new Execute<Func> (filename, func);
	}
	virtual int operator()(iterator begin, iterator end) {
		return m_func(begin, end);
	}
};

//互換性を保つために作成した関数ポインター版(非推奨)
using func_ptr = int(*)(iterator,iterator);
template<> class Execute<func_ptr>:public FileBase {
	func_ptr m_func;
	Execute(const std::string& name, func_ptr func) :
			FileBase(name), m_func(func) {
	}
public:
	static Execute<func_ptr>* Create(const std::string& filename, const func_ptr& func) {
		return new Execute<func_ptr>(filename, func);
	}
	virtual int operator()(iterator begin, iterator end) {
		return m_func(begin, end);
	}
};
//毎回書くのが面倒なので省略形
template <class T> FileBase* CreateExecute(const std::string& filename,const T& obj){
	return Execute<T>::Create(filename,obj);
}

class Integer: public FileBase {
private:
	int32_t* data;
	Integer(const std::string& filename, int32_t* d);
public:
	static Integer* Create(const std::string& filename, int32_t* d = nullptr);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);
};

class Float: public FileBase {
private:
	float* data;
	Float(const std::string& filename, float* f);
public:
	static Float* Create(const std::string& filename, float* f = nullptr);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);
};

class String: public FileBase {
private:
	std::string* data;
	String(const std::string& filename, std::string* str = nullptr);
public:
	static String* Create(const std::string& filename, std::string* str);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);
};

class Fix: public FileBase {
private:
	fix32* data;
	Fix(const std::string& filename, fix32* f = nullptr);
public:
	static Fix* Create(const std::string& filename, fix32* f);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);
};

class Property: public FileBase {
private:
	std::function<std::string(void)> fget;
	std::function<std::string(const std::string&)> fset;
	Property(const std::string& filename,
			const std::function<std::string(void)>& get,
			const std::function<std::string(const std::string&)>& set);
public:
	static Property* Create(const std::string& filename,
			const std::function<std::string(void)>& get,
			const std::function<std::string(const std::string&)>& set);
	static Property* CreateReadOnly(const std::string& filename,
			const std::function<std::string(void)>& get);
	static Property* CreateWriteOnly(const std::string& filename,
			const std::function<std::string(const std::string&)>& set);
	virtual std::string GetData();
	virtual std::string SetData(const std::string&);

};

extern Directory *root, *current;

} /* namespace App */
}
#endif /* APP_FILE_H_ */
