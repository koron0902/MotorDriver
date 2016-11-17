#include <text.hpp>
#include <memory>
#include <sstream>
#include <cstdio>

using namespace std;

namespace common {

vector<string> Split(const string& text, const string& seq) {
	vector<string> arg;
	string line;
	
	for (char s : text) {
		bool flag = false; //分割フラグ
		for (char cmp : seq) {
			if (cmp == s) {
				flag = true;
				break;
			}
		}

		if (!flag) {
			line += s;
		} else {
			if (line != "") {
				arg.push_back(line);
				line.clear();
			}
		}
	}

	if (line != "") {
		arg.push_back(line);
	}

	return move(arg);
}

string Space(const string& str, unsigned int s) {
	string ans = "";
	for (unsigned int i = 0; i < s; i++) {
		ans += str;
	}
	return ans;
}

string ToStr(int32_t value){
	char buf[16];
	snprintf(buf,sizeof(buf),"%d",value);
	return string(buf);
}
/*
string ToStr(float value){
	char buf[16];
	snprintf(buf,sizeof(buf),"%f",value);
	return string(buf);
}
*/
} /* namespace common */
