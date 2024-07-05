#pragma once


#define HIDE_PREFIX L"$van"

#define HIDE_PREFIX_LENGTH (sizeof(HIDE_PREFIX) / sizeof(WCHAR) - 1)


BOOL HasPrefix(LPCWSTR str);
BOOL HasPrefixU(UNICODE_STRING str);