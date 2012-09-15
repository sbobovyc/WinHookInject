#ifndef MYLIB_H
#define MYLIB_H

#include <windows.h>

#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )

//I could just use PROCESS_ALL_ACCESS but it's always best to use the absolute bare minimum of priveleges, so that your code works in as
//many circumstances as possible.
#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)
//#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE)


#ifdef __cplusplus
extern "C" {
#endif

bool DllExport IsWindowsNT();
unsigned long DllExport GetTargetProcessIdFromWindow(char *, char *);
unsigned long DllExport GetTargetProcessIdFromProcname(char *);
bool DllExport LoadDll(char *, char *);
bool DllExport InjectDLL(DWORD, char *);

#ifdef __cplusplus
}
#endif

#endif /* MYLIB_H */

