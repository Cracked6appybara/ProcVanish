#pragma once

#define          LoadLibraryA_CRC32				         0x3FC1BD8D
#define          NtQuerySystemInformation_CRC32          0x97FD2398

#define          kernel32dll_CRC32       0x6AE69F02
#define          ntdlldll_CRC32          0x84C05E40



typedef NTSTATUS(WINAPI* fnNtQuerySystemInformation)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
    );



typedef NTSTATUS(NTAPI* fnNtQueryDirectoryFile)(
    HANDLE                 FileHandle,
    HANDLE                 Event,
    PIO_APC_ROUTINE        ApcRoutine,
    PVOID                  ApcContext,
    PIO_STATUS_BLOCK       IoStatusBlock,
    PVOID                  FileInformation,
    ULONG                  Length,
    FILE_INFORMATION_CLASS FileInformationClass,
    BOOLEAN                ReturnSingleEntry,
    PUNICODE_STRING        FileName,
    BOOLEAN                RestartScan
    );

typedef NTSTATUS(NTAPI* fnNtQueryDirectoryFileEx)(
    HANDLE                 FileHandle,
    HANDLE                 Event,
    PIO_APC_ROUTINE        ApcRoutine,
    PVOID                  ApcContext,
    PIO_STATUS_BLOCK       IoStatusBlock,
    PVOID                  FileInformation,
    ULONG                 Length,
    FILE_INFORMATION_CLASS FileInformationClass,
    ULONG                  QueryFlags,
    PUNICODE_STRING        FileName
    );


typedef HMODULE(WINAPI* fnLoadLibraryA)(LPCSTR lpLibFileName);