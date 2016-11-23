#pragma once

#include <string>
#include <text.hpp>
namespace Middle {
namespace XPort{
//USB、UARTに対しての命令(主に通知用)

void Init();

bool IsEmpty();

void Write(const std::string&);
void Write(char);

static inline void WriteLine(){
	Write(common::newline);
}

static inline void WriteLine(const std::string& text){
	Write(text);
	WriteLine();
}

void Claer();

void Flush();//送信同期



}

}
