#include <XPort.hpp>
#include <USB.hpp>
#include <Uart.hpp>
#include <simple_buffer.hpp>
#include <error.hpp>
#include <ring_buffer.hpp>

using namespace Device;
using namespace common;

namespace Middle {
namespace XPort {
const PortFlag PortFlag::None = 0, PortFlag::USB = 1, PortFlag::UART = 2,
		PortFlag::USB_UART = 3;
static PortFlag port;
static SimpleBuffer<TxBufferSize> txbuf;
static uint8_t rxbuf_raw[RxBufferSize];
static RINGBUFF_T rxbuf;

void Init() {
	txbuf.Reset();
	port = PortFlag::UART;
	RingBuffer_Init(&rxbuf, rxbuf_raw, sizeof(rxbuf_raw[0]),
			sizeof(rxbuf_raw) / sizeof(rxbuf_raw[0]));
}

PortFlag GetPort() {
	return port;
}

void SetPort(PortFlag v) {
	port = v;
}

bool IsEnableUSB() {
	return USB::IsConnected();
}

void Write(char c) {
	txbuf.Write((uint8_t) c);
	if (txbuf.GetDepth() > TxBufferLimit) {
		Flush();
	}
}

void Write(const uint8_t* data, size_t sz) {
	if (txbuf.GetDepth() + sz > TxBufferSize) {
		Flush();
	}
	txbuf.Write((uint8_t*) data, min(sz, TxBufferSize));
	if (txbuf.GetDepth() > TxBufferLimit) {
		Flush();
	}
}

void Write(const char* text) {
	Write((uint8_t*) text, strlen(text));
}

void Write(const std::string& text) {
	Write((uint8_t*) text.data(), text.length());
}

void Write(const std::vector<char>& vec) {
	Write((uint8_t*) vec.data(), vec.size());
}

void WriteLine() {
	Write(newline);
}

void WriteLine(const char* str) {
	Write(str);
	WriteLine();
}

void WriteLine(const std::string& text) {
	Write(text + newline);
}
bool IsEmpty() {
	bool flag = true;
	if (port.IsUart()) {
		flag &= Uart::IsEmpty();
	}
	if (port.IsUSB()) {
		flag &= USB::IsEmpty();
	}
	return flag;
}

void Claer() {
	Uart::Claer();
	USB::Claer();
}

void Flush() {
	if (port.IsUSB()) {
		USB::Write(txbuf.GetData(), txbuf.GetDepth());
	}
	if (port.IsUart()) {
		//Uart::Write(txbuf.GetData(),txbuf.GetDepth());
		Uart::Write(txbuf.GetData(), txbuf.GetDepth());
	}
	txbuf.Reset();
}

bool IsExist(char c) {
	if (port.IsSingle()) {
		if (port.IsUSB()) {
			return USB::IsExist(c);
		}
		if (port.IsUart()) {
			return Uart::IsExist(c);
		}
	}
	return false;
}

std::string ReadLine() {
	if (port.IsSingle()) {
		if (port.IsUSB()) {
			return USB::ReadLine();
		}
		if (port.IsUart()) {
			return USB::ReadLine();
		}
	}
	return "";
}

std::string Read() {
	if (port.IsSingle()) {
		if (port.IsUSB()) {
			return USB::Read();
		}
		if (port.IsUart()) {
			return USB::Read();
		}
	}
	return "";
}




}
}

