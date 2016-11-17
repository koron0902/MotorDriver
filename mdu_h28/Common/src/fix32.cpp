#include <fix32.hpp>
#include <string>

using namespace std;

namespace common {

const fix32 fix32::Max = fix32::CreateRaw(INT32_MAX);
const fix32 fix32::Min = fix32::CreateRaw(INT32_MIN);
const fix32 fix32::Eps = fix32::CreateRaw(1);
}
/*
ostream& operator<<(ostream& os, const common::fix32 fix) {
	//面倒なのでflaotで実装しているが重いなら違う実装をしてね。
	os << (float) fix;
	return os;
}

istream & operator >>(istream & is, common::fix32 & fix) {
	// 面倒なのでflaotで実装しているが重いなら違う実装をしてね。
	float x;
	is >> x;
	fix = (common::fix32) x;
	return is;
}

*/
