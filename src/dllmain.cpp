#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // A process is loading the DLL.
        break;
    case DLL_THREAD_ATTACH:
        // A thread is being created.
        break;
    case DLL_THREAD_DETACH:
        // A thread is exiting cleanly.
        break;
    case DLL_PROCESS_DETACH:
        // A process is unloading the DLL.
        break;
    }
    return TRUE;
}