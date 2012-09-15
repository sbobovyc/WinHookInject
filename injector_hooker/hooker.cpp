#include <stdio.h>
#include "libutils.h"

int main(int argc, char ** argv) {

    OutputDebugString("Starting up");
    if(IsWindowsNT()) {
        unsigned long pid = GetTargetProcessIdFromProcname((char *)"MinGW Win32.exe");
        printf("PID: %u\n", pid);
        LoadDll((char *)"MinGW Win32.exe", (char *)"inject.dll");
    } else {
        MessageBox(0, "Your system does not support this method", "Error!", 0);
    }

    return 0;	
}

