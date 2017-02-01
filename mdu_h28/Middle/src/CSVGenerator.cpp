/*
 * CSVGenerator.cpp
 *
 *  Created on: 2017/02/01
 *      Author: Keita
 */

#include <CSVGenerator.hpp>
#include <ff.hpp>
#include <string>

namespace Middle{
	namespace CSV{
		FATFS mFatFs;
		FIL mFil;
		UINT bw;
		std::string buf;
		constexpr uint32_t MaxSize = 10240;

		void Init(){
			buf = "";
		}

		void Add2Buf(const std::string& str){
			if(buf.length() + str.length() > MaxSize)
				return;
			buf += str;
		}

		void Generate(const std::string& _path){
			std::string path = "0:/" + _path + ".csv";
			FatFs::f_mount(&mFatFs, "0:", 0);
			FatFs::f_open(&mFil, path.c_str(), FA_CREATE_ALWAYS | FA_WRITE);
			FatFs::f_write(&mFil, buf.c_str(), buf.length(), &bw);
			FatFs::f_close(&mFil);
			FatFs::f_mount(NULL, "0:", 0);
		}
	}
}
