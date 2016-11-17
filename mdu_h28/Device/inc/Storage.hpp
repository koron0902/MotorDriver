#include <stdint.h>
#include <unit.hpp>
#include <string>
#include <vector>


#ifndef DEVICE_STORAGE_H_
#define DEVICE_STORAGE_H_

namespace Device {
namespace Storage{

using pos_t =uint32_t;
constexpr pos_t FileSize=1024;

using namespace common;
enum class EFileID:uint32_t{
	Config=0_KiByte,
	Log=1_KiByte,
	Report=2_KiByte,
	Error=3_KiByte,
};

void Init();

class EFile{
	pos_t m_base;
	pos_t m_pos;
public:
	EFile(EFileID id);
	EFile(const EFile&)=default;
	virtual ~EFile()=default;

	inline void SetPos(pos_t pos){m_pos=pos;}
	inline bool IsOverrun()const{return m_pos>=FileSize;}
	inline void Skip(size_t size){m_pos+=size;}
	template <class T> inline void Skip(){m_pos+=sizeof(T);}
	//method
	void Write(const char* data,size_t size);
	void Read(char* data,size_t size);


};




}
} /* namespace Device */

#endif /* DEVICE_STORAGE_H_ */
