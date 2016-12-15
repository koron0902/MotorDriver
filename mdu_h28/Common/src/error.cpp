#include "error.hpp"
namespace common{

const char* GetMessage(int err){
	switch (err) {
		case 0:
			return "OK";
		default:
			return "FoundOut";
	}
}

}
