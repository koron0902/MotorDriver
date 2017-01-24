#pragma once
#ifndef DEVICE_INC_INT_HPP_
#define DEVICE_INC_INT_HPP_

#include <type.hpp>
#include <Port.hpp>

namespace Device{
namespace INT{
using namespace common;
//割り込みピンとして登録する
enum class INT_ID
	:uint32_t {
		INT0 = 0, INT1, INT2, INT3, INT4, INT5, INT6, INT7
};
extern const uint32_t IntMax;//内部のピン数

void Init();
//割り込みを登録する
using IntHandler= std::function<void(INT_ID)>;
void SetHandler(INT_ID id,const IntHandler& func=nullptr,uint8_t priority=254);

/*
 * 内部の操作用(ただのラッパー)
 */

//物理ピンを内部ピンとして登録する。
void SetInt(Port::PortData data, INT_ID idx);
//エッジ割り込みに設定する。
void ModeEdge(INT_ID id);
//High,Raise割り込みを設定する
void ModeHigh(INT_ID id);
//Low,Raise割り込みを設定する。
void ModeLow(INT_ID id);
//状態変化割り込みに設定する
void ModeChanged(INT_ID id);
//立ち下がりに設定する
void ModeEdgeLow(INT_ID id);
//立ち上がりに設定する
void ModeEdgeHigh(INT_ID id);
}
}


#endif /* DEVICE_INC_INT_HPP_ */
