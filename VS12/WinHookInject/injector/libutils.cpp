#include <windows.h>
#include <TlHelp32.h>
#include <Shlwapi.h>
#include <stdio.h>

#include "libutils.h"

#pragma comment(lib,"shlwapi.lib")

bool IsWindowsNT()
{
    // check current version of Windows
    DWORD version = GetVersion();
    // parse return
    DWORD majorVersion = (DWORD)(LOBYTE(LOWORD(version)));
    DWORD minorVersion = (DWORD)(HIBYTE(LOWORD(version)));

    printf("Version is %d.%d\n", 
            majorVersion,
            minorVersion);

    return (version < 0x80000000);
}

unsigned long GetTargetProcessIdFromWindow(char *className, char *windowName)
{
    unsigned long procID;
    HWND targetWnd;

    targetWnd = FindWindow(className, windowName);
    GetWindowThreadProcessId(targetWnd, &procID);

    return procID;
}

unsigned long GetTargetProcessIdFromProcname(char *procName)
{
    PROCESSENTRY32 pe;
    HANDLE thSnapshot;
    BOOL retval, ProcFound = false;

    thSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(thSnapshot == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, "Error: unable to create toolhelp snapshot", "Loader", MB_OK);
        return false;
    }

    pe.dwSize = sizeof(PROCESSENTRY32);

    retval = Process32First(thSnapshot, &pe);

    while(retval)
    {
        if(StrStrI(pe.szExeFile, procName) )
        {
            ProcFound = true;
            break;
        }

        retval    = Process32Next(thSnapshot,&pe);
        pe.dwSize = sizeof(PROCESSENTRY32);
    }
    if(ProcFound) {
        return pe.th32ProcessID;
    } else {
        return 0;
    }
}

bool LoadDll(char *procName, char *dllName)
{
    DWORD ProcID = 0;

    ProcID = GetTargetProcessIdFromProcname(procName);

    if(!(InjectDLL(ProcID, dllName)))
        MessageBox(NULL, "Process located, but injection failed", "Loader", MB_OK);

    return true;
}



bool InjectDLL(DWORD ProcessID, char *dllName)
{
    HANDLE Proc;
    HANDLE Thread;
    char buf[50]={0};
    LPVOID RemoteString, LoadLibAddy;

    if(!ProcessID)
        return false;

    char* DirPath = new char[MAX_PATH];
    char* FullPath = new char[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, DirPath);
    sprintf_s(FullPath, 128, "%s\\%s", DirPath, dllName);
    printf("Loading %s\n", FullPath);

    Proc = OpenProcess(CREATE_THREAD_ACCESS, FALSE, ProcessID);

    if(!Proc)
    {
        sprintf_s(buf, 128, "OpenProcess() failed: %d", GetLastError());
        MessageBox(NULL, buf, "Loader", MB_OK);
        return false;
    }

    LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(FullPath), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(Proc, (LPVOID)RemoteString, FullPath, strlen(FullPath), NULL);
    Thread = CreateRemoteThread(Proc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, 0, NULL);   
    if(Thread == NULL) {
        sprintf_s(buf, 128, "CreateRemoteThread() failed: %d", GetLastError());
        MessageBox(NULL, buf, "Loader", MB_OK);
        return false;
    } 
    CloseHandle(Proc);
    
    delete [] DirPath;
    delete [] FullPath;
    return true;
}

