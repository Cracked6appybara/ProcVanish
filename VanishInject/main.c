#include <stdio.h>
#include <winternl.h>
#include <Windows.h>
#include "resource.h"

typedef struct BASE_RELOCATION_BLOCK {
    DWORD PageAddress;
    DWORD BlockSize;
} BASE_RELOCATION_BLOCK, *PBASE_RELOCATION_BLOCK;

typedef struct BASE_RELOCATION_ENTRY {
    USHORT Offset : 12;
    USHORT Type : 4;
} BASE_RELOCATION_ENTRY, *PBASE_RELOCATION_ENTRY;

BOOL GetDLLFromRsrc(PBYTE *pPayload, DWORD* pdwSize)
{

    HRSRC hRsrcBlockHandle;
    HGLOBAL hHandleToPointer;
    DWORD dwDataSize;
    PBYTE pAddress = NULL;
    
    printf("[i] Grabbing the handle to the information block of the resource... ");
    if (!(hRsrcBlockHandle = FindResourceA(NULL, MAKEINTRESOURCEA(IDR_VANISH1), "VANISH"))) {
        printf("FindResourceA Failed\t| %d\n", GetLastError());
        return FALSE;
    }
    printf("[+] DONE!!!\n");

    printf("[i] Getting the size of the data stored inside the resource... ");
    if (!(dwDataSize = SizeofResource(NULL, hRsrcBlockHandle)))
    {
        printf("SizeofResource Failed\t| %d\n", GetLastError());
        return FALSE;
    }
    printf("[+] DONE!!!\n");
    
    printf("[i] Grabbing the handle to the pointer... ");
    if (!(hHandleToPointer = LoadResource(NULL, hRsrcBlockHandle)))
    {
        printf("LoadResource Failed\t| %d\n", GetLastError());
        return FALSE;
    }
    printf("[+] DONE!!!\n");

    if (!(pAddress = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwDataSize)))
    {
        printf("HeapAlloc Failed\t| %d\n", GetLastError());
        return FALSE;
    }

    memcpy(pAddress, hHandleToPointer, dwDataSize);

    *pdwSize = dwDataSize;
    *pPayload = pAddress;
    
    return TRUE;
}

BOOL ParsingHeaders(IN PBYTE pFile, IN DWORD dwSize) {

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFile;
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
        return FALSE;

    PIMAGE_NT_HEADERS pNtHdrs = (PIMAGE_NT_HEADERS)(pFile + pDosHeader->e_lfanew);
    if (pNtHdrs->Signature != IMAGE_NT_SIGNATURE)
        return FALSE;

    IMAGE_DATA_DIRECTORY importDir = pNtHdrs->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
    DWORD_PTR relocationTable = importDir.VirtualAddress + (DWORD)pFile;
    DWORD relocationsProcessed = 0;

    while (relocationsProcessed < importDir.Size)
    {
        PBASE_RELOCATION_BLOCK relocationBlock = (PBASE_RELOCATION_BLOCK)(relocationTable + relocationsProcessed);
        relocationsProcessed += sizeof(BASE_RELOCATION_BLOCK);
        DWORD relocationsCount = (relocationBlock->BlockSize - sizeof(BASE_RELOCATION_BLOCK)) / sizeof(BASE_RELOCATION_ENTRY);
        PBASE_RELOCATION_ENTRY relocationEntries = (PBASE_RELOCATION_ENTRY)(relocationTable + relocationsProcessed);

        for (DWORD i = 0; i < relocationsCount; i++)
        {
            relocationsProcessed += sizeof(BASE_RELOCATION_ENTRY);

            if (relocationEntries[i].Type == 0)
            {
                continue;
            }

            DWORD_PTR relocationRVA = relocationBlock->PageAddress + relocationEntries[i].Offset;
            DWORD_PTR addressToPatch = 0;
            ReadProcessMemory(GetCurrentProcess(), (LPCVOID)((DWORD_PTR)pFile + relocationRVA), &addressToPatch, sizeof(DWORD_PTR), NULL);
            memcpy((PVOID)((DWORD_PTR)pFile + relocationRVA), &pFile, sizeof(DWORD_PTR));
        }
    }
    
}

int main(void) {

    PBYTE pData = NULL;
    DWORD dwSize = 0;

    if (!GetDLLFromRsrc(&pData, &dwSize)) {
        return -1;
    }
    printf("[i] DLL Stored At\t| 0x%p | SIZE %d\n", pData, dwSize);


    
    printf("[#] Press <Enter> To Quit...\n");
    getchar();
    
    return 0;
}