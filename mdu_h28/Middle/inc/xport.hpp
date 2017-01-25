#pragma once
#include <stdint.h>
#include <string>
#include <text.hpp>
#include <bits.hpp>

namespace Middle {
namespace XPort {
//USB、UARTに対しての命令(主に通知用)

//入出力先に切り替えようの列挙体もどき
struct PortFlag {
private:
	uint32_t m_data;
public:
	const static PortFlag None, USB, UART, USB_UART;
	PortFlag(){}
	constexpr PortFlag(uint32_t data) :
			m_data(data) {
	}

	constexpr PortFlag(const PortFlag&) = default;

	inline PortFlag operator =(PortFlag inst) {
		return PortFlag(m_data = inst.m_data);
	}

	//集合和
	inline PortFlag operator +(PortFlag inst) const {
		return PortFlag(m_data | inst.m_data);
	}
	inline PortFlag operator +=(PortFlag inst) {
		return PortFlag(m_data |= inst.m_data);
	}
	//集合差
	inline PortFlag operator -(PortFlag inst) const {
		return PortFlag(m_data & ~inst.m_data);
	}

	inline PortFlag operator -=(PortFlag inst) {
		return PortFlag(m_data &= ~inst.m_data);
	}

	bool IsUSB() const {
		return m_data & USB.m_data;
	}
	bool IsUart() const {
		return m_data & UART.m_data;
	}
	bool IsClosed() const {
		return m_data == None.m_data;
	}
	bool IsSingle()const{
		return common::BitCount(m_data)==1;
	}
};

void Init();

//Portを開放する  (失敗時には負数)
PortFlag GetPort();
void SetPort(PortFlag);
bool IsEnableUSB();

constexpr size_t TxBufferSize=512;//書き込みバッファー
constexpr size_t TxBufferLimit=256;//Flushサイズ
constexpr size_t RxBufferSize=256;//読み込みバッファのサイズ
constexpr size_t RxBufferLimit=16;//読み取りサイズ

bool IsEmpty();
void Write(char);
void Write(const uint8_t* data, size_t sz);
void Write(const char*);
void Write(const std::string& text);
void Write(const std::vector<char>& vec);
void WriteLine();
void WriteLine(const char*);
void WriteLine(const std::string& text);

std::string ReadLine();
std::string Read();

void Claer();
void Flush(); //送信同期

bool IsExist(char);
static inline bool IsLine(){
	return IsExist(common::newline);
}
bool IsEnableUSB();


std::string ReadLine();
std::string Read();




}

}
