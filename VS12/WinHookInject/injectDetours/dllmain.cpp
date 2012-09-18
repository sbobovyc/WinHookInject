// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"


// Target pointer for the uninstrumented 
//
int (WINAPI *pMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT) = MessageBoxA;
int  WINAPI MyMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uiType);

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        DetourRestoreAfterWith();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)pMessageBoxA, MyMessageBoxA);
        DetourTransactionCommit();
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)pMessageBoxA, MyMessageBoxA);
        DetourTransactionCommit();
    }
    return TRUE;
}

int  WINAPI MyMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uiType)
{
    OutputDebugString("In MyMessageBoxA");
    lpCaption = "Hooked!";
    int retValue = pMessageBoxA(hWnd, lpText, lpCaption, uiType);
    return retValue;
}