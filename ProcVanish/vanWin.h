#pragma once

CHAR _toUpper(CHAR C);
UINT32 CRC32B(LPCSTR cString);
SIZE_T StringLengthA(IN LPCSTR String);
void* __cdecl memcpy(void* pDestination, void* pSource, size_t sLength);
void* __cdecl memset(void* pTarget, int value, size_t cbTarget);