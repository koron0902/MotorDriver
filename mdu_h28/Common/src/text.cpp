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

	vector<char> sp;
	sp.reserve(32);
	int64_t a = common::abs(value);
	while (a > 0) {
		sp.push_back(ToChar(a % 10));
		a /= 10;
	}

	if (sp.empty()) {
		return "0";
	} else {
		string ans=sign(value)?"-":"";
		do {
			ans += sp.back();
			sp.pop_back();
		} while (!sp.empty());
		return ans;
	}

}

std::string ToStr(fix32 value) {
	constexpr static int32_t d[] { (1 << 16) / 10, (1 << 16) / 100, (1 << 16)
			/ 1000, (1 << 16) / 10000 };

	int32_t num, point;
	fix32 a= abs(value);
	num = a.GetInt();
	point = a.GetPoint();

	string ans = ToStr(num) + ".";
	uint idx;
	for (idx=0;idx<4-1;idx++) {
		ans+=ToChar(point/d[idx]);
		point%=d[idx];
	}
	ans+=ToChar(value/d[idx]);
	return ans;
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
