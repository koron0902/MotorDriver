#pragma once
#ifndef ZERO_SET_HEADER
#define ZERO_SET_HEADER

#include <string.h>
namespace common{
template <class T>void ZeroSet(T& obj){
	memset((void*)&obj,0,sizeof(obj));
}
}

#endif
