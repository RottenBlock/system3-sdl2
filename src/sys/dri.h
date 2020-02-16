/*
	ALICE SOFT SYSTEM 3 for Win32

	[ DRI ]
*/

#ifndef _DRI_H_
#define _DRI_H_

#include "../common.h"

class DRI
{
public:
	DRI() {}
	~DRI() {}

	uint8* load(const char* path, int page, int* size);
	uint8* load_mda(uint32 crc32, int page, int* size);
};

#endif
