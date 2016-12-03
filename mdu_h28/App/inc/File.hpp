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
		SetType(FileType::Execute);
	}
public:
	static Execute<Func>* Create(const std::string& filename,
			const Func& func) {
		return new Execute<Func>(filename, func);
	}
	virtual int operator()(iterator begin, iterator end) {
		return m_func(begin, end);
	}
};

//互換性を保つために作成した関数ポインター版(非推奨)
using func_ptr = int(*)(iterator,iterator);
template<> class Execute<func_ptr> : public FileBase {
	func_ptr m_func;
	Execute(const std::string& name, func_ptr func) :
			FileBase(name), m_func(func) {
		SetType(FileType::Execute);

	}
public:
	static Execute<func_ptr>* Create(const std::string& filename,
			const func_ptr& func) {
		return new Execute<func_ptr>(filename, func);
	}
	virtual int operator()(iterator begin, iterator end) {
		return m_func(begin, end);
	}
};
//毎回書くのが面倒なので省略形
template<class T> FileBase* CreateExecute(const std::string& filename,
		const T& obj) {
	return Execute<T>::Create(filename, obj);
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
/*
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
 */

template<class FSet, class FGet> class Property: public FileBase {
private:
	FSet m_set;
	FGet m_get;
	Property(const std::string& filename, const FSet& set, const FGet& get) :
			FileBase(filename), m_set(set), m_get(get) {
			SetType(FileType::Properties);

	}
public:
	static Property<FSet, FGet>* Create(const std::string& filename,
			const FSet& set, const FGet& get) {
		return new Property<FSet, FGet>(filename, set, get);
	}
	virtual std::string GetData() {
		return m_get();
	}
	virtual std::string SetData(const std::string& value) {
		return m_set(value);
	}
};

template<class FSet, class FGet> Property<FSet, FGet>* CreateProperty(
		const std::string & name, const FSet& set, const FGet& get) {
	return new Property<FSet, FGet>(name, set, get);
}

template<class FGet> class ReadOnlyProperty: public FileBase {
private:
	FGet m_get;
	ReadOnlyProperty(const std::string filename, const FGet& get) :
			FileBase(filename), m_get(get) {
		SetType(FileType::Properties);

	}
public:
	static ReadOnlyProperty<FGet>* Create(const std::string & filename,
			const FGet& get) {
		return new ReadOnlyProperty<FGet>(filename, get);
	}
	virtual std::string GetData() {
		return m_get();
	}
};

template<class FGet> ReadOnlyProperty<FGet>*CreateReadOnlyProperty(
		const std::string& name, const FGet& get) {
	return ReadOnlyProperty<FGet>::Create(name, get);
}

template<class FSet> class WriteOnlyProperty: public FileBase {
private:
	FSet m_set;
	WriteOnlyProperty(const std::string& name, const FSet& set) :
			FileBase(name), m_set(set) {
		SetType(FileType::Properties);

	}
public:
	static WriteOnlyProperty<FSet>* Create(const std::string & filename,
			const FSet& set) {
		return new WriteOnlyProperty<FSet*>(filename, set);
	}
	virtual std::string SetData(const std::string& value){
		return m_set(value);
	}
};

template<class FSet> WriteOnlyProperty<FSet>*CreateWriteOnlyProperty(
		const std::string& name, const FSet& set) {
	return WriteOnlyProperty<FSet>::Create(name, set);
}


/*
 template<> class Property<>: public FileBase {
 private:
 FSet m_set;
 FGet m_get;
 Property(const std::string& filename, const FSet& set, const FGet& get) :
 FileBase(filename), m_set(set), m_get(get) {
 }
 public:
 static Property<FSet, FGet>* Create(const std::string& filename,
 const FSet& set, const FGet& get) {
 return new Property<FSet, FGet>(filename, set, get);
 }
 virtual std::string GetData() {
 return m_get();
 }
 virtual std::string SetData(const std::string& value) {
 return m_set(value);
 }
 };
 */


extern Directory *root, *current;

} /* namespace App */
}
#endif /* APP_FILE_H_ */
