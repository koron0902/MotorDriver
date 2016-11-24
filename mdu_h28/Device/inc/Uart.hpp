/*
 * Uart.h
 *
 *  Created on: 2016/11/04
 *      Author: TERU
 */

#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_

#include <File.hpp>
#include <stdint.h>
#include <string>
#include <vector>
#include <text.hpp>
namespace Device {

namespace Uart {



void Init();
void TxEnable(bool);

bool IsFull();
//書き込み命令
void Write(char);
void Write(const uint8_t*, size_t);
void Write(const char*);
void Write(const std::vector<uint8_t>&);
void Write(const std::string&);
//書き込み命令(改行あり)
static inline void WriteLine() {
	Write(common::newline);
}
static inline void WriteLine(const std::string& line) {
	Write(line);
	WriteLine();
}
static inline void WriteLine(const char * line) {
	Write(line);
	WriteLine();
}
static inline void WriteLine(const std::vector<uint8_t>& bytes) {
	Write(bytes);
	WriteLine();
}

bool IsEmpty();
char ReadByte();
std::string Read(size_t sz);
std::string ReadLine();
void Claer();//Read Bufferを削除
bool IsBusy();
}



}
#endif /* DRIVER_UART_H_ */
