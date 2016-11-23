#pragma once

#include <memory>
#include <algorithm>
#include <fix.hpp>



namespace common {

template<class T, unsigned int X, unsigned int Y> struct Matrix {
	using uint=unsigned int;
	using Mat=Matrix<T,X,Y>;
	constexpr static uint size = X * Y;
	constexpr static uint None=std::min(X,Y);

	T dat[X * Y];

	T& operator ()(uint i, uint j) {
		return dat[i + j * X];
	}

	const T& operator ()(uint i, uint j) const {
		return dat[i + j * X];
	}

	T& operator [](uint idx) {
		return dat[idx];
	}

	const T& operator [](uint idx) const {
		return dat[idx];
	}

	//クラップされた入出力
	T& at(uint i, uint j) {
		return operator ()(i % X, j % Y);
	}

	const T& at(uint i, uint j) const {
		return operator ()(i % X, j % Y);
	}

	//基本的な初期化
	Matrix() {
		//何もしない
	}

	Matrix(const T& value) {
		for (uint i = 0; i < size; i++) {
			dat[i] = value;
		}
	}

	Matrix(const Matrix&)=default;


	static Mat Unit(const T& value) {
		Mat temp(T(0));
		for (uint idx = 0; idx < std::min(X, Y); idx++) {
			temp(idx, idx) = value;
		}
		return std::move(temp);
	}



	//基本演算子
	const Matrix& operator =(const Matrix& inst) {
		for (uint i = 0; i < size; i++) {
			dat[i] = inst[i];
		}
		return *this;
	}

	const Matrix& operator +=(const Matrix& inst) {
		for (uint i = 0; i < size; i++) {
			dat[i] += inst[i];
		}
		return *this;
	}

	const Matrix& operator -=(const Matrix& inst) {
		for (uint i = 0; i < size; i++) {
			dat[i] -= inst[i];
		}
		return *this;
	}

	const Matrix& operator +=(const T& value) {
		for (uint i = 0; i < size; i++) {
			dat[i] += value;
		}
		return *this;

	}

	const Matrix& operator -=(const T& value) {
		for (uint i = 0; i < size; i++) {
			dat[i] -= value;
		}
		return *this;

	}

	const Matrix& operator *=(const T& value) {
		for (uint i = 0; i < size; i++) {
			dat[i] *= value;
		}
		return *this;
	}

	Matrix operator +(const Matrix& inst)const {
		Matrix<T,X,Y> temp;
		for (uint idx = 0; idx < size; idx++) {
			temp[idx] = operator[](idx) + inst[idx];
		}
		return std::move(temp);
	}

	Matrix operator -(const Matrix& inst) const{
			Matrix<T,X,Y> temp;
			for (uint idx = 0; idx < size; idx++) {
				temp[idx] = operator[](idx) - inst[idx];
			}
			return std::move(temp);
		}

	//転置行列
	Matrix<T, Y, X> operator !() const {
		Matrix<T, Y, X> temp;
		for (uint j = 0; j < Y; j++) {
			for (uint i = 0; i < X; i++) {
				temp(j, i) = (*this)(i, j);
			}
		}
		return std::move(temp);
	}

	template<unsigned Z> Matrix<T, X, Z> operator *(
			 const Matrix<T, Y, Z>& inst)const {
		Matrix<T, X, Z> temp;
		T sum;
		uint i,j,k;
		for ( i = 0; i < X; i++) {
			for ( j = 0; j < Z; j++) {
				sum = T(0);
				for ( k = 0; k < Y; k++) {
					sum +=  (*this)(i, k) * inst(k, j);
				}
				temp(i, j) = sum;
			}
		}
		return std::move(temp);
	}
};

extern template class Matrix<fix32,4,1>;
extern template class Matrix<fix32,1,4>;

}
