#include <windows.h>
#include <stdio.h>



CONST CHAR* g_cAPINames[] = {
	"LoadLibraryA",
	"NtQuerySystemInformation",
	NULL
};


#define HASH "CRC32"


UINT32 CRC32B(LPCSTR cString) {

	UINT32      uMask = 0x00,
		uHash = 0xFFFFFFFF;
	INT         i = 0x00;

	while (cString[i] != 0) {

		uHash = uHash ^ (UINT32)cString[i];

		for (int ii = 0; ii < 8; ii++) {

			uMask = -1 * (uHash & 1);
			uHash = (uHash >> 1) ^ (0xEDB88320 & uMask);
		}

		i++;
	}

	return ~uHash;
}




int main() {

	for (int i = 0; g_cAPINames[i] != NULL; i++)
		printf("#define \t %s_%s \t 0x%0.8X\n", g_cAPINames[i], HASH, CRC32B(g_cAPINames[i]));

	printf("#define \t %s_%s \t 0x%0.8X\n", "kernel32dll", HASH, CRC32B("kernel32.dll"));
	printf("#define \t %s_%s \t 0x%0.8X\n", "ntdlldll", HASH, CRC32B("ntdll.dll"));

	return 0;
}