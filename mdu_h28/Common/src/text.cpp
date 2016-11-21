#include <text.hpp>
#include <memory>
#include <stack>
#include <cstdio>
#include <qmath.hpp>

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

string ToStr(int64_t value) {
	string ans = "";
	ans.reserve(64);
	std::stack<char> sp;

	int a = common::abs(value);
	bool sign = common::sign(value);

	while (a > 0) {
		sp.push(ToChar(a % 10));
		a /= 10;
	}

	if (sp.empty()) {
		return "0";
	} else {
		if (!sign)ans='-';
		do{
			ans+=sp.top();
			sp.pop();
		}while (!sp.empty());
		return ans;
	}

}

fix32 ToFix(const std::string& text) {
	uint32_t idx = 0;
	bool sign = false; //マイナスならtrue
	char c;
	auto next = [&text,&idx]()->char {
		return text[idx++];
	};
	fix32 fix(0);
	c = next();
	if (IsSign(c)) {
		sign = (c == '-');
		c = next();
	}
	while (IsNumber(c)) {
		fix *= 10;
		fix += FromNumber(c);
		c = next();
	}
	if (IsPoint(c)) {
		fix32 s = fix32::CreateInt(1);
		c = next();
		while (IsNumber(c)) {
			s /= 10;
			fix += s * FromNumber(c);
			c = next();
		}
	}

	if (sign) {
		fix = -fix;
	}

	return fix;
}

int32_t ToInt(const std::string& text) {
	uint32_t idx = 0;
	bool sign = false;
	char c;
	auto next = [&text,&idx]()->char {
		return text[idx++];
	};
	int32_t num = 0;
	c = next();
	if (IsSign(c)) {
		sign = (c == '-');
		c = next();
	}
	while (IsNumber(c)) {
		num *= 10;
		num += FromNumber(c);
		c = next();
	}

	if (sign) {
		num = -num;
	}

	return num;
}
} /* namespace common */
