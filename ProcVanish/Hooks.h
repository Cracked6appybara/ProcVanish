#pragma once

BOOL InitialiseHooks();

BOOL InstallHook(LPCSTR dll, LPCSTR function, LPVOID* originalFunction, LPVOID hookedFunction, LPVOID* newFunction);

BOOL HasPrefix(LPCWSTR str);
BOOL HasPrefixU(UNICODE_STRING str);



NTSTATUS WINAPI hookedNtQuery(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);