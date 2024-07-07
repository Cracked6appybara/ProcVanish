#include <windows.H>
#include <stdio.h>
#include "structs.h"
#include "vanWin.h"
#include "TypeDef.h"




HMODULE GetModuleHandleH(IN UINT32 uModuleHash) {


	PPEB                    pPeb = NULL;
	PPEB_LDR_DATA           pLdr = NULL;
	PLDR_DATA_TABLE_ENTRY   pDte = NULL;

	pPeb = (PPEB)__readgsqword(0x60);
	pLdr = (PPEB_LDR_DATA)(pPeb->Ldr);
	pDte = (PLDR_DATA_TABLE_ENTRY)(pLdr->InMemoryOrderModuleList.Flink);

	// Return the handle of the local .exe image
	if (!uModuleHash)
		return (HMODULE)pDte->InInitializationOrderLinks.Flink;

	while (pDte) {

		if (pDte->FullDllName.Buffer && pDte->FullDllName.Length < MAX_PATH) {

			CHAR    cLDllName[MAX_PATH] = { 0 };
			DWORD   x = 0x00;

			while (pDte->FullDllName.Buffer[x]) {

				CHAR	wC = pDte->FullDllName.Buffer[x];

				// Convert to lowercase
				if (wC >= 'A' && wC <= 'Z')
					cLDllName[x] = wC - 'A' + 'a';
				// Copy other characters (numbers, special characters ...)
				else
					cLDllName[x] = wC;

				x++;
			}

			cLDllName[x] = '\0';

			if (CRC32B(pDte->FullDllName.Buffer) == uModuleHash || CRC32B(cLDllName) == uModuleHash)
				return (HMODULE)pDte->InInitializationOrderLinks.Flink;
		}

		// Move to the next node in the linked list
		pDte = *(PLDR_DATA_TABLE_ENTRY*)(pDte);
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FARPROC GetProcAddressH(IN HMODULE hModule, IN UINT32 uApiHash) {

	PBYTE                           pBase = (PBYTE)hModule;
	PIMAGE_NT_HEADERS               pImgNtHdrs = NULL;
	PIMAGE_EXPORT_DIRECTORY         pImgExportDir = NULL;
	PDWORD                          pdwFunctionNameArray = NULL;
	PDWORD                          pdwFunctionAddressArray = NULL;
	PWORD                           pwFunctionOrdinalArray = NULL;
	DWORD                           dwImgExportDirSize = 0x00;

	if (!hModule || !uApiHash)
		return NULL;

	pImgNtHdrs = (PIMAGE_NT_HEADERS)(pBase + ((PIMAGE_DOS_HEADER)pBase)->e_lfanew);
	if (pImgNtHdrs->Signature != IMAGE_NT_SIGNATURE)
		return NULL;

	pImgExportDir = (PIMAGE_EXPORT_DIRECTORY)(pBase + pImgNtHdrs->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	dwImgExportDirSize = pImgNtHdrs->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	pdwFunctionNameArray = (PDWORD)(pBase + pImgExportDir->AddressOfNames);
	pdwFunctionAddressArray = (PDWORD)(pBase + pImgExportDir->AddressOfFunctions);
	pwFunctionOrdinalArray = (PWORD)(pBase + pImgExportDir->AddressOfNameOrdinals);

	for (DWORD i = 0; i < pImgExportDir->NumberOfFunctions; i++) {

		CHAR* pFunctionName = (CHAR*)(pBase + pdwFunctionNameArray[i]);
		PVOID	pFunctionAddress = (PVOID)(pBase + pdwFunctionAddressArray[pwFunctionOrdinalArray[i]]);

		if (CRC32B(pFunctionName) == uApiHash) {

			// Forwarded functions support:
			if ((((ULONG_PTR)pFunctionAddress) >= ((ULONG_PTR)pImgExportDir)) &&
				(((ULONG_PTR)pFunctionAddress) < ((ULONG_PTR)pImgExportDir) + dwImgExportDirSize)
				) {

				CHAR	cForwarderName[MAX_PATH] = { 0 };
				DWORD	dwDotOffset = 0x00;
				PCHAR	pcFunctionMod = NULL;
				PCHAR	pcFunctionName = NULL;

				memcpy(cForwarderName, pFunctionAddress, StringLengthA((PCHAR)pFunctionAddress));

				for (int i = 0; i < StringLengthA((PCHAR)cForwarderName); i++) {

					if (((PCHAR)cForwarderName)[i] == '.') {
						dwDotOffset = i;
						cForwarderName[i] = NULL;
						break;
					}
				}

				pcFunctionMod = cForwarderName;
				pcFunctionName = cForwarderName + dwDotOffset + 1;

				fnLoadLibraryA pLoadLibraryA = (fnLoadLibraryA)GetProcAddressH(GetModuleHandleH(kernel32dll_CRC32), LoadLibraryA_CRC32);
				if (pLoadLibraryA)
					return GetProcAddressH(pLoadLibraryA(pcFunctionMod), CRC32B(pcFunctionName));
			}
			return (FARPROC)pFunctionAddress;
		}

	}

	return NULL;
}