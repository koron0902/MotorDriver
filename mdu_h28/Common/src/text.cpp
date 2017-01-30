#include <text.hpp>
#include <memory>
#include <stack>
#include <cstdio>
#include <qmath.hpp>
#include <token.hpp>
using namespace std;

namespace common {

char newline ='\r';
char comma=',';


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
		string ans = sign(value) ? "-" : "";
		do {
			ans += sp.back();
			sp.pop_back();
		} while (!sp.empty());
		return ans;
	}

}

std::string ToStr(fix32 value) {
	constexpr static int32_t d[]{(1 << 16) / 10, (1 << 16) / 100, (1 << 16)
			/ 1000, (1 << 16) / 10000 };

	int32_t num, point;
	fix32 a = abs(value);
	num = a.GetInt();
	point = a.GetPoint();

	string ans = ToStr(num) + ".";
	uint idx;
	for (idx = 0; idx < 4 - 1; idx++) {
		ans += ToChar(point / d[idx]);
		point %= d[idx];
	}
	ans += ToChar(value / d[idx]);
	return ans;
}

std::string ToStrF(float raw) {
	return ToStr(fix32::CreateFloat(raw));
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

template<class T> T ToInt(const std::string& text) {
	uint32_t idx = 0;
	bool sign = false;
	char c;
	auto next = [&text,&idx]()->char {
		return text[idx++];
	};
	T num = 0;
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

template<class T> T ToUInt(const std::string& text) {
	uint32_t idx = 0;
	char c;
	auto next = [&text,&idx]()->char {
		return text[idx++];
	};
	T num = 0;
	c = next();
	while (IsNumber(c)) {
		num *= 10;
		num += FromNumber(c);
		c = next();
	}
	return num;
}

int32_t ToInt32(const std::string& text) {
	return ToInt<int32_t>(text);
}

int64_t ToInt64(const std::string& text) {
	return ToInt<int64_t>(text);
}

uint32_t ToUInt32(const std::string& text) {
	return ToUInt<uint32_t>(text);
}

uint64_t ToUInt64(const std::string& text) {
	return ToUInt<uint64_t>(text);
}

float ToFloat(const std::string& text) {
	uint32_t idx = 0;
	bool sign = false; //マイナスならtrue
	char c;
	auto next = [&text,&idx]()->char {
		return text[idx++];
	};
	float raw = 0.0f;
	c = next();
	if (IsSign(c)) {
		sign = (c == '-');
		c = next();
	}
	while (IsNumber(c)) {
		raw *= 10.0f;
		raw += FromNumber(c);
		c = next();
	}
	if (IsPoint(c)) {
		float s = 1.0f;
		c = next();
		while (IsNumber(c)) {
			s /= 10.0f;
			raw += s * FromNumber(c);
			c = next();
		}
	}

	if (sign) {
		return -raw;
	} else {
		return raw;
	}
}

bool IsNumberPattern(const std::string& text) {
	auto it = text.begin();
	if (IsEnd(*it)) return false; //nullは論外
	//符号はなくてもいい
	if (IsSign(*it)) {
		it++;
		if (IsEnd(*it)) return false;
	}

	while (!IsEnd(*it)) {
		if (IsNumber(*it)) {
			it++;
		} else if (IsPoint(*it)) {
			it++;
			while (IsEnd(*it)) {
				if (IsNumber(*it)) {
					it++;
				} else {
					return false;
				}
			}
			return true;
		} else {
			return false;
		}
	};
	return true;
}

bool IsUnsignedNumberPatten(const std::string& text) {
	auto it = text.begin();
	if (IsEnd(*it)) return false; //nullは論外
	if (*it=='+'){
		it++;
		if (IsEnd(*it))return false;
	}
	//必ず一文字は数字
	if (IsNumber(*it)){
		it++;
		if (IsEnd(*it))return false;
	}else{
		return false;
	}

	while (IsEnd(*it)){
		if (IsNumber(*it)){
			it++;
		}else{
			return false;
		}
	}
	return true;
}

bool IsOptionPattern(const std::string& text) {
	auto it = text.begin();
	if (IsEnd(*it)) return false; //nullは論外
	if (*it != '-') return false;
	it++;
	while (!IsEnd(*it)) {
		if (IsNumber(*it) || IsAlphabet(*it)) {
			it++;
		} else {
			return false;
		}
	}
	return true;
}

CmdParser cmdParser;

CmdParser::CmdParser(){
	this->Clear();
}

bool CmdParser::Parse(text_iterator begin, text_iterator end){
	this->Clear();

	if(*begin ==  "")
		return false;

	this->cmd = *begin;

	while(begin != end){
		begin++;
		if(IsOptionPattern(*begin)){
			this->option[this->len++] = *begin;
			if(begin == end)
				break;
			if(!IsOptionPattern(*(begin + 1))){
				begin++;
				this->value[this->len - 1] = *begin;
			}
		}
	}
	return true;
}

bool CmdParser::Search(const std::string& opt, uint32_t* index){
	if(index == nullptr)
		return false;
	if(opt == "")
		return false;
	if(IsEnd(*(opt.begin())))
		return false;
	if(this->option[0] == "")
		return false;

	for(uint i = 0; i < OptionSize; i++){
		if(option[i] == opt){
			*index = i;
			return true;
		}
	}
	return false;
}

void CmdParser::Clear(){
	this->cmd = "";
	this->len = 0;
	for(uint i = 0; i < OptionSize; i++){
		this->option[i] = "";
		this->value[i] = "";
	}
}

bool CmdParser::IsOptionNull(){
	return (this->len == 0);
}

const std::string CmdParser::GetOptionArg(uint32_t index){
	return this->value[index];
}

} /* namespace common */
