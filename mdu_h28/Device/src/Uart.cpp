/*
 * Uart.cpp
 *
 *  Created on: 2016/11/04
 *      Author: TERU
 */

#include <chip.hpp>
#include <ring_buffer.hpp>
#include <cstring>
#include <Uart.hpp>
#include <configuration.hpp>
using namespace std;

static constexpr size_t TxSize = 64, RxSize = 64; //バッファーのサイズ
static RINGBUFF_T TxBuf, RxBuf;
static char TxRaw[TxSize], RxRaw[RxSize];

namespace Device {

namespace Uart {
//補助関数
inline static void interrupt_disable() {
	NVIC_DisableIRQ(UART0_IRQn);
}

inline static void interrupt_enable() {
	interrupt_disable();
	NVIC_SetPriority(UART0_IRQn, common::PriorityUart);
	NVIC_EnableIRQ(UART0_IRQn);
}

//実装
void Init() {
	//Clock Supply
	Chip_Clock_SetUARTBaseClockRate(Chip_Clock_GetMainClockRate(), false);
	Chip_Clock_SetUARTFRGDivider(1);
	Chip_UART_Init(LPC_USART0);
	//Configuration
	Chip_UART_ConfigData(LPC_USART0,
	UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1);
	Chip_UART_SetBaud(LPC_USART0, 115200);
	//Buffer Initialize
	RingBuffer_Init(&TxBuf, &TxRaw, sizeof(char), TxSize);
	RingBuffer_Init(&RxBuf, &RxRaw, sizeof(char), RxSize);
	//割り込み処理
	Chip_UART_IntEnable(LPC_USART0, UART_INTEN_RXRDY | UART_INTEN_TXRDY);
	interrupt_enable();
	//実験のためにTxを有効にする。
	TxEnable(true);
	Chip_UART_Enable(LPC_USART0);

}

void TxEnable(bool fag) {
	if (fag) {
		Chip_UART_TXEnable(LPC_USART0);
	} else {
		Chip_UART_TXDisable(LPC_USART0);
	}
}

bool IsFull() {
	return RingBuffer_IsFull(&TxBuf);
}

void Write(char c) {
	while (IsFull())
		;
	Chip_UART_SendRB(LPC_USART0, &TxBuf, &c, sizeof(char));
}

void Write(const uint8_t* data, size_t sz) {
	Chip_UART_SendRB(LPC_USART0, &TxBuf, data, sz);
}

void Write(const char* str) {
	Write((uint8_t*) str, std::strlen(str));
}

void Write(const vector<uint8_t>& data) {
	Write(data.data(), data.size());
}

void Write(const string& text) {
	Write((uint8_t*) text.data(), text.length());
}

bool IsEmpty() {
	return RingBuffer_IsEmpty(&RxBuf);
}

char ReadByte() {
	char c;
	while (RingBuffer_IsEmpty(&RxBuf)) {
		__NOP();
	}
	Chip_UART_ReadRB(LPC_USART0, &RxBuf, &c, sizeof(char));
	return c;
}

string Read(size_t sz) {
	string s = "";
	while (sz--) {
		s += ReadByte();
	}
	return s;
}

string ReadLine() {
	string s = "";
	char c;
	while ((c = ReadByte()) != '\r') {
		s += c;
	}
	return s;
}

void Claer() {
	RingBuffer_Flush(&RxBuf);
}

bool IsBusy() {
	return !RingBuffer_IsEmpty(&TxBuf);
}

bool IsExist(char c) {
	char* data = (char*) RxRaw;
	for (unsigned int i = RxBuf.tail; i != RxBuf.head;
			i = (i + 1) % RxBuf.count) {
		if (data[i] == c) return true;
	}
	return false;
}

extern "C" void UART0_IRQHandler(void) {
	Chip_UART_IRQRBHandler(LPC_USART0, &RxBuf, &TxBuf);
}

}

} /* namespace Device */
