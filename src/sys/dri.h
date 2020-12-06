/*
	ALICE SOFT SYSTEM 3 for Win32

	[ DRI ]
*/

#ifndef _DRI_H_
#define _DRI_H_

#include <windows.h>
#include <stdlib.h>
#include "../common.h"

class DRI
{
public:
	DRI() {}
	~DRI() {}

	uint8 *load(const char *file_name, int page, int *size);
	uint8 *load_mda(HINSTANCE hInst, uint32 crc32_a, uint32 crc32_b, int page, int *size);
};

#endif
