// hooker.cpp : Defines the entry point for the console application.
//

#include "libutils.h"
#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	OutputDebugString("Starting up");
    if(IsWindowsNT()) {
        unsigned long pid = GetTargetProcessIdFromProcname((char *)"guiTest.exe");
        printf("PID: %u\n", pid);
        LoadDll((char *)"guiTest.exe", (char *)"inject.dll");
    } else {
        MessageBox(0, "Your system does not support this method", "Error!", 0);
    }
	return 0;
}

