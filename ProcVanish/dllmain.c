// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include "structs.h"
#include "common.h"
#include "Hooks.h"


#pragma comment(lib, "minhook.x64.lib")
#pragma comment(lib, "Shlwapi.lib")




BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:

        InstallHook();
        
        break;
    case DLL_PROCESS_DETACH:
        
        if (!UnHook()) {
            return FALSE;
        }

        break;
    }

    return TRUE;
}
