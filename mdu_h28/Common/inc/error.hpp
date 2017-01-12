#pragma once
#ifndef COMMON_ERROR_H_
#define COMMON_ERROR_H_

namespace common {

enum Code{
	OK=0,
	Unknown=-1,
	PtrErr=-100,
	FoundOut,
	Empty,
	AccessErr,

	MissMatch=-10000
};

const char* GetMessage(int);

} /* namespace common */

#endif /* COMMON_ERROR_H_ */
