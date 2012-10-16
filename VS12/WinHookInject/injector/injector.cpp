// hooker.cpp : Defines the entry point for the console application.
//

#include "libutils.h"
#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 3) {
		printf("Usage: %s program.exe library.dll\n", argv[0]);
		return 0;
	}
	OutputDebugString("Starting up");
    if(IsWindowsNT()) {
        //unsigned long pid = GetTargetProcessIdFromProcname((char *)"guiTest.exe");
		unsigned long pid = GetTargetProcessIdFromProcname(argv[1]);
		if(pid == 0) {
			MessageBox(0, "Process not found", "Error!", 0);
			return 0;
		}
        printf("PID: %u\n", pid);
        LoadDll(argv[1], argv[2]);
    } else {
        MessageBox(0, "Your system does not support this method", "Error!", 0);
    }
	return 0;
}

