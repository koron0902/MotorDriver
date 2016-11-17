#include <chip.h>
#include <string.h>
#include <stdio.h>
#include <Storage.hpp>

using namespace std;

namespace Device {
namespace Storage{

void Init(){
	//supply clock
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_EEPROM);
	Chip_SYSCTL_PeriphReset(RESET_EEPROM);

	//test
	const char *str="hello world\r";
	char buf[32];
	EFile file(EFileID::Config);
	file.Write(str,strlen(str));
	file.Read(buf,32);

	printf("%s",buf);


}

EFile::EFile(EFileID id){
	m_base= 0x00000100;
	m_pos=0;
}

void EFile::Write(const char* data,size_t size){
	uint8_t ret=
	Chip_EEPROM_Write(m_base+m_pos,(uint8_t*)data,size);
}

void EFile::Read(char* data,size_t size){
	uint8_t ret=
	Chip_EEPROM_Read(m_base+m_pos,(uint8_t*)data,size);
}






}
} /* namespace Device */
