#ifndef INJECT_H
#define INJECT_H

#include <windows.h>

#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )

#ifdef __cplusplus
extern "C" {
#endif
DllExport void test();

BOOL APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved);

#ifdef __cplusplus
}
#endif

#endif /* INJECT_H */
