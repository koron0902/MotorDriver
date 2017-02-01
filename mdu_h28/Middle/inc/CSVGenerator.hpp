/*
 * CSVGenerator.hpp
 *
 *  Created on: 2017/02/01
 *      Author: Keita
 */

#ifndef MIDDLE_INC_CSVGENERATOR_HPP_
#define MIDDLE_INC_CSVGENERATOR_HPP_

#include <stdint.h>
#include <string>

namespace Middle{
	namespace CSV{
		void Init();
		void Add2Buf(const std::string& str);
		void Generate(const std::string& _path);
	}
}


#endif /* MIDDLE_INC_CSVGENERATOR_HPP_ */
