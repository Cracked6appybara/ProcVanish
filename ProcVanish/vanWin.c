#include <Windows.h>
#include <stdio.h>
#include "vanWin.h"

CHAR _toUpper(CHAR C)
{
	if (C >= 'a' && C <= 'z')
		return C - 'a' + 'A';

	return C;
}



UINT32 CRC32B(LPCSTR cString) {

	UINT32      uMask = 0x00,
		uHash = 0xFFFFFFFF;
	INT         i = 0x00;

	while (cString[i] != 0) {

		uHash = uHash ^ (UINT32)cString[i];

		for (int ii = 0; ii < 8; ii++) {

			uMask = -1 * (uHash & 1);
			uHash = (uHash >> 1) ^ (0xEDB88320 & uMask);
		}

		i++;
	}

	return ~uHash;
}


SIZE_T StringLengthA(IN LPCSTR String) {

	LPCSTR String2;

	for (String2 = String; *String2; ++String2);

	return (String2 - String);
}

extern void* __cdecl memcpy(void*, void*, size_t);
#pragma intrinsic(memcpy)
#pragma function(memcpy)
void* __cdecl memcpy(void* pDestination, void* pSource, size_t sLength) {

	PBYTE D = (PBYTE)pDestination;
	PBYTE S = (PBYTE)pSource;

	while (sLength--)
		*D++ = *S++;

	return pDestination;
}

extern void* __cdecl memset(void*, int, size_t);
#pragma intrinsic(memset)
#pragma function(memset)
void* __cdecl memset(void* pTarget, int value, size_t cbTarget) {
	unsigned char* p = (unsigned char*)pTarget;
	while (cbTarget-- > 0) {
		*p++ = (unsigned char)value;
	}
	return pTarget;
}