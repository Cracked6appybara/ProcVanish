#include <stdio.h>
#include <winternl.h>
#include <Windows.h>
#include "resource.h"

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

#define EXPORTED_FUNC_NAME "ReflectiveFunction"

#define ALLOC(SIZE)				LocalAlloc(LPTR, (SIZE_T)SIZE)
#define FREE(BUFF)				LocalFree((LPVOID)BUFF)
#define REALLOC(BUFF, SIZE)		LocalReAlloc(BUFF, SIZE,  LMEM_MOVEABLE | LMEM_ZEROINIT)

DWORD RVA2Offset(IN DWORD dwRVA, IN PBYTE pBase) {

    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)(pBase + ((PIMAGE_DOS_HEADER)pBase)->e_lfanew);
    PIMAGE_SECTION_HEADER pImgSectionHdr = (PIMAGE_SECTION_HEADER)((PBYTE)&pNTHeader->OptionalHeader + pNTHeader->FileHeader.SizeOfOptionalHeader);

    // Iterates throught sections
    for (int i = 0; i <pNTHeader->FileHeader.NumberOfSections; i++)
    {
        // If the RVA is located inside "i" PE Section
        if (dwRVA >= pImgSectionHdr[i].VirtualAddress && dwRVA < (pImgSectionHdr[i].VirtualAddress + pImgSectionHdr[i].Misc.VirtualSize))
            return (dwRVA - pImgSectionHdr[i].VirtualAddress) + pImgSectionHdr[i].PointerToRawData;
    }

    printf("\t[ERROR] Couldn't Convert The 0x%0.8X RVA To File Offset!\n");
    return 0x00;
}

DWORD GetReflectiveFunctionOffset(IN ULONG_PTR uRflDllBuffer) {
	
    PIMAGE_NT_HEADERS			pImgNtHdrs					= NULL;
    PIMAGE_EXPORT_DIRECTORY		pImgExportDir				= NULL;
    PDWORD						pdwFunctionNameArray		= NULL;
    PDWORD						pdwFunctionAddressArray		= NULL;
    PWORD						pwFunctionOrdinalArray		= NULL;

    pImgNtHdrs = (uRflDllBuffer + ((PIMAGE_DOS_HEADER)uRflDllBuffer)->e_lfanew);
    if (pImgNtHdrs->Signature != IMAGE_NT_SIGNATURE)
        return 0x00;
		
    pImgExportDir			= ( PIMAGE_EXPORT_DIRECTORY ) (uRflDllBuffer + RVA2Offset(pImgNtHdrs->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, uRflDllBuffer));
    pdwFunctionNameArray	= ( PDWORD ) (uRflDllBuffer + RVA2Offset(pImgExportDir->AddressOfNames, uRflDllBuffer));
    pdwFunctionAddressArray	= ( PDWORD ) (uRflDllBuffer + RVA2Offset(pImgExportDir->AddressOfFunctions, uRflDllBuffer));
    pwFunctionOrdinalArray	= ( PWORD )  (uRflDllBuffer + RVA2Offset(pImgExportDir->AddressOfNameOrdinals, uRflDllBuffer));


    for (DWORD i = 0; i < pImgExportDir->NumberOfFunctions; i++){

        PCHAR pcFunctionName = (PCHAR)(uRflDllBuffer + RVA2Offset(pdwFunctionNameArray[i], uRflDllBuffer));
		
        if (strcmp(pcFunctionName, EXPORTED_FUNC_NAME) == 0) 
            return RVA2Offset(pdwFunctionAddressArray[pwFunctionOrdinalArray[i]], uRflDllBuffer);
    }

    printf("\t[!] Cound'nt Resolve %s's Offset! \n", EXPORTED_FUNC_NAME);
    return 0x00;
}

int main(void) {

    PBYTE pData = NULL;
    DWORD dwSize = 0;

    if (!GetDLLFromRsrc(&pData, &dwSize)) {
        return -1;
    }
    printf("[i] DLL Stored At\t| 0x%p | SIZE %d\n", pData, dwSize);

GetReflectiveFunctionOffset(pData);
    
    printf("[#] Press <Enter> To Quit...\n");
    getchar();
    
    return 0;
}