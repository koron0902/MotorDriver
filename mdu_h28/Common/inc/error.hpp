#pragma once
namespace common{
namespace errors{
//ここにエラーコードを書いていく
constexpr int
 OK=0,
 UnKnown=-1,
 Null=-2,
 FoundOut=-3,
 InputBad=-4,
 PromiseBad=-5
;

const char* GetMessage(int);
static inline bool IsError(int code){
	return code<0;
}


}


};
