#include "error.hpp"
namespace common{
namespace errors{

struct pair{
	const int code;
	const char*const message;
};


const static pair tag[]={
		{OK,"OK"},
		{UnKnown,"UnKnown"},
		{Null,"Null"},
		{InputBad,"InputBad"},
		{PromiseBad,"Promise Error"}
};

const char* GetMessage(int err){
	for (auto& it:tag){
		if (it.code==err){
			return it.message;
		}
	}
	return nullptr;
}
}
}
