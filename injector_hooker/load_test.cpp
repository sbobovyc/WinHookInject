#include <stdio.h>
#include <windows.h>


int main() {

    FARPROC funcPtr;
    OutputDebugString("Starting up");
    HMODULE hl = LoadLibraryA("inject.dll");
    if(hl == NULL) {
        OutputDebugString("loading failed");
    } 
    funcPtr = GetProcAddress(hl, "test");
    if(funcPtr == NULL) {
        OutputDebugString("function not found");
    }
    funcPtr(); 
    return 0;
}
