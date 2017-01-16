#pragma once
#ifndef  	_TOKEN_HEADER_GUARD_
#define 	_TOKEN_HEADER_GUARD_

namespace common{
//トークン処理用
extern char newline;

static auto IsMark=[](char c){
	return ('!'<=c&&c<='/')||(':'<=c&&c<='?')||
			('['<=c&&c<='`')||('{'<=c&&c<='~');
};

static auto IsEndLine=[](char c){
	return c==newline;
};

static auto IsEnd=[](char c)->bool{
	return c=='\0';
};

static auto IsPoint=[](char c)->bool{
	return c=='.';
};

static auto IsSign=[](char c){
	return c=='+'||c=='-';
};

static auto IsSmall=[](char c){
	return 'a'<=c&&c<='z';
};

static auto IsLarge=[](char c){
	return 'A'<=c&&c<='Z';
};

static auto IsAlphabet=[](char c){
	return IsSmall(c)||IsLarge(c);
};

static auto IsNumber=[](char c)->bool{
	return '0'<=c&&c<='9';
};

static auto IsHex=[](char c){
	return  ('0'<=c&&c<='9')||('a'<=c&&c<='f')||('A'<=c&&c<='F');
};




}

#endif
