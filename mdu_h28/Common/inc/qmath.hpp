#ifndef COMMON_INC_QMATH_HPP_
#define COMMON_INC_QMATH_HPP_

#include <bits.hpp>
#include <type.hpp>

namespace common {

template<class T> T abs(const T& a) {
	if (a >= T(0)) {
		return a;
	} else {
		return -a;
	}
}

template<class T> bool sign(const T& c) {
	return c >= T(0);
}

template<class T> T clip(T x, T s, T e) {
	if (x > s) x = s;
	else if (x < e) x = e;
	return x;
}

//値域を意味する。
template<class T>
class region {
	T m_start, m_end;
public:
	constexpr region(const T& start, const T& end) :
			m_start(start), m_end(end) {
	}
	region(const region&) = default;

	//値を取得したり与えたりする
	const T& GetStart() const {
		return m_start;
	}

	const T& GetEnd() const {
		return m_end;
	}

	void SetStart(const T& value) {
		m_start = value;
	}

	void SetEnd(const T& value) {
		m_end = value;
	}

	//
	bool In(const T& cmp) const {
		return (m_start < cmp) && (cmp < m_end);
	}

	bool InWide(const T& cmp) const {
		return (m_start <= cmp) && (cmp <= m_end);
	}

	bool Out(const T& cmp) const {
		return (cmp < m_start) || (m_end < cmp);
	}

	bool OutWide(const T&cmp) const {
		return (cmp <= m_start) || (m_end <= m_end);
	}

	//値域の中に収める
	T Fit(const T& value) const {
		if (m_start > value) return m_start;
		else if (value > m_end) return m_end;
		else return value;
	}
};

//よく使うであろう型を実体化しておく
extern template class region<int32_t> ;
extern template class region<fix32> ;

namespace regions {
extern const region<fix32> one; //|x|<=1
extern const region<fix32> percent;//0<=x<=1
}
}

#endif /* COMMON_INC_QMATH_HPP_ */
