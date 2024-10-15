#pragma once

VOID InitHooks();

BOOL InstallHook();
BOOL UnHook();



NTSTATUS WINAPI hookedNtQuery(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);