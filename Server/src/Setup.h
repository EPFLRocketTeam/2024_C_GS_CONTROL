

#ifndef SETUP_H
#define SETUP_H

#include <QString>
#include <cstdint>
#include <vector>
#include <stdlib.h>

namespace auth {
	typedef struct AuthKey_t {
		QString key;
		uint8_t accessRight; // Access [X X X X X X R W], X are unused bits, bit 0 is Write authorization and stands for Read
		
	} AuthKey;
	const std::vector<AuthKey> validKeys = {{"ClientSecretKey1", 3}, {"ClientSecretKey2", 2}};
}

#endif
