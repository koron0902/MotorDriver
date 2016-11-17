#pragma once

//意図的にnamespaceを使用していない

template <class T> void Release(T*& ptr){
	if (ptr!=nullptr){
		delete ptr;
		ptr=nullptr;
	}
}
