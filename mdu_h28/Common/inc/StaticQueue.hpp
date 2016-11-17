#ifndef __SQUEUE_HEADER
#define __SQUEUE_HEADER
#include <stdint.h>
#include <stddef.h>
#include <memory>
//固定領域のキューである。
//制約としてsizeは2の階乗にすること
namespace common {
template<class T, size_t size>
class StaticQueue {
public:
	constexpr static uint32_t Mask = size - 1;
private:
	T m_data[size];
	uint32_t m_start, m_end, m_depth;
public:
	void Reset() {
		m_start = m_end = m_depth = 0;
	}

	StaticQueue() {
		Reset();
	}

	void Push(const T& data) {
		if (m_depth < size) {
			m_data[m_start] = data;
			m_start = (m_start + 1) & Mask;
			m_depth++;
		} else {

		}
	}

	const T& Top() {
		return m_data[m_end];
	}

	void Pop() {
		if (m_depth > 0) {
			m_end = (m_end + 1) & Mask;
		} else {

		}
	}

	T TopAndPop(){
		T temp=Top();
		Pop();
		return std::move(temp);
	}

	//プロパティ
	size_t GetDepth() const {
		return m_depth;
	}
	size_t GetSize() const {
		return size;
	}

	bool IsFull() const {
		return m_depth >= size;
	}

	bool IsEmpty() const {
		return m_depth == 0;
	}

};
}
#endif
