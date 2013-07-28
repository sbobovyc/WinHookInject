#include <windows.h>

#define MYFUNC_ADDR 0x01341FF0

DWORD WINAPI MyThread(LPVOID);
DWORD g_threadID;
HMODULE g_hModule;
void __stdcall CallFunction(void);

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
    switch(Reason)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hDLL;
        DisableThreadLibraryCalls(hDLL);
        CreateThread(NULL, NULL, &MyThread, NULL, NULL, &g_threadID);
    break;
    case DLL_THREAD_ATTACH:
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

DWORD WINAPI MyThread(LPVOID)
{
    while(true)
    {
        if(GetAsyncKeyState(VK_F2) & 1)
        {
            CallFunction();
        }
        else if(GetAsyncKeyState(VK_F3) &1)
            break;
    Sleep(100);
    }
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}

void __stdcall CallFunction(void)
{
    typedef void (__stdcall *pFunctionAddress)(void);
    pFunctionAddress pMySecretFunction = (pFunctionAddress)(MYFUNC_ADDR);
    pMySecretFunction();
}