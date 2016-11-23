#pragma once
#include <memory>
using namespace std;

namespace common{
template <class T,uint X,uint Y>string ToStr(const Matrix<T,X,Y>& mat){
	string temp="";
	uint i,j;
	for (j=0;j<Y;j++){
		for (i=0;i<X-1;i++){
			temp+=ToStr(mat(i,j))+",";
		}
		temp+=ToStr(mat(i,j))+newline;
	}
	return move(temp);
}

}
