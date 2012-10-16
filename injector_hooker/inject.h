#ifndef INJECT_H
#define INJECT_H

#include <windows.h>

#ifdef BUILD_DLL
    #define DllExport __declspec(dllexport)
#else
    #define DllExport __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" 
{
#endif
DllExport void test();

BOOL APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved);

#ifdef __cplusplus
}
#endif

#endif /* INJECT_H */
