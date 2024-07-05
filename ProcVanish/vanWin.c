#include <Windows.h>
#include <stdio.h>
#include "vanWin.h"


LPVOID GetFunction(LPCWSTR dll, LPCSTR function) {

	HMODULE module = GetProcAddress(GetModuleHandleA(dll), function);				// CHANGE THIS TO CUSTOM FUNCTION


	return module;
}