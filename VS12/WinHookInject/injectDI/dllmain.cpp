//http://www.rohitab.com/discuss/topic/34411-run-time-directx-hooking-using-code-injection-and-vtable/
// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"


#ifndef __cplusplus
extern "C" {
#endif

#pragma comment (lib, "dinput8.lib") 
HRESULT (WINAPI *oldDirectInput8Create) (HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter) = DirectInput8Create;
HRESULT (WINAPI *oldCreateDevice) (LPVOID *ppvOut, REFGUID rguid, LPDIRECTINPUTDEVICE * lplpDirectInputDevice, LPUNKNOWN pUnkOuter) = NULL;
HRESULT (WINAPI *oldGetDeviceState) (LPVOID IDirectInputDevice8W, DWORD cbData,LPVOID lpvData) = NULL;
HRESULT WINAPI myDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter);
HRESULT WINAPI myCreateDevice(LPVOID *ppvOut, REFGUID rguid, LPDIRECTINPUTDEVICE * lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
HRESULT WINAPI myGetDeviceState(LPVOID IDirectInputDevice8W, DWORD cbData,LPVOID lpvData);
//__declspec(dllexport) void DetourAndErrorCheck(PVOID* pFunctionToDetour, PVOID pReplacement, const char* functionName);
DWORD* GetVtableAddress(void* pObject);
__declspec(dllexport) void dummy(void);

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {
	if (DetourIsHelperProcess()) {
		return TRUE;
	}
	
	char buffer[100];
	if (dwReason == DLL_PROCESS_ATTACH) {
		OutputDebugString("Hooking");	
		DetourRestoreAfterWith();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());		
		oldDirectInput8Create = (HRESULT(WINAPI *)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter))DetourFindFunction("dinput8.dll", "DirectInput8Create");
		sprintf_s(buffer, 100, "DirectInput8Create at %x", oldDirectInput8Create);
		OutputDebugString(buffer);
		if(oldDirectInput8Create == NULL)
			OutputDebugString("Failed to find function");
		int error = DetourAttach(&(PVOID&)oldDirectInput8Create, myDirectInput8Create);
		if(error != NO_ERROR)
			OutputDebugString("Failed to detour");
		error = DetourTransactionCommit();
		if(error != NO_ERROR)
			OutputDebugString("Failed to commit");
	}
	else if (dwReason == DLL_PROCESS_DETACH) {
		OutputDebugString("Unhooking");
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)oldDirectInput8Create, myDirectInput8Create);
		DetourDetach(&(PVOID&)oldCreateDevice, myCreateDevice);
		DetourDetach(&(PVOID&)oldGetDeviceState, myGetDeviceState);
		DetourTransactionCommit();
	}
	return TRUE;
}

HRESULT WINAPI myDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter) {
	char buffer[100];
	OutputDebugString("Hooked DirectInput8Create");	
	HRESULT retValue = oldDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	// get pointer to direct input interface, find CreateDevice and hook it	
	// we need to do some pointer chaising through the vtable to find the real function 
	LPVOID  ptrCreateDevice = *(((LPVOID *)(*(LPVOID *)(*(LPVOID *)ppvOut)))+3);
	sprintf_s(buffer, 100, "CreateDevice is at %x", ptrCreateDevice);
	OutputDebugString(buffer);	

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());	
	oldCreateDevice = (HRESULT (WINAPI *) (LPVOID *, REFGUID , LPDIRECTINPUTDEVICE * , LPUNKNOWN )) ptrCreateDevice;
	DetourAttach(&(PVOID&)oldCreateDevice, myCreateDevice);
	DetourTransactionCommit();

	return retValue;
}

HRESULT WINAPI myCreateDevice(LPVOID *ppvOut, REFGUID rguid, LPDIRECTINPUTDEVICE * lplpDirectInputDevice, LPUNKNOWN pUnkOuter) {
	OutputDebugString("Hooked CreateDevice");	
	HRESULT retValue = oldCreateDevice(ppvOut, rguid, lplpDirectInputDevice, pUnkOuter);
	// get input device and pointer to GetDeviceState and hook it
	LPVOID  ptrGetDeviceState= *(((LPVOID *)(*(LPVOID *)(*(LPVOID *)lplpDirectInputDevice)))+9);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());	
	oldGetDeviceState = (HRESULT (WINAPI *) (LPVOID, DWORD ,LPVOID )) ptrGetDeviceState;
	DetourAttach(&(PVOID&)oldGetDeviceState, myGetDeviceState);
	DetourTransactionCommit();
	return retValue;
}

HRESULT WINAPI myGetDeviceState(LPVOID IDirectInputDevice8W, DWORD cbData,LPVOID lpvData) {
	//OutputDebugString("Hooked GetDeviceState");	
	HRESULT retValue = oldGetDeviceState(IDirectInputDevice8W, cbData, lpvData);
	// modify data
	*((char *)lpvData+DIK_LEFT) = 0;
	*((char *)lpvData+DIK_RIGHT) = 0;
	// a more convinient way to reference the data
	char * keystate = (char *)lpvData;
	if(keystate[DIK_A] & 0x80) {
		keystate[DIK_LEFT] = 0x80;
	}
	if(keystate[DIK_F] & 0x80) {
		keystate[DIK_RIGHT] = 0x80;
	}
	return retValue;
}


__declspec(dllexport) void dummy(void) {}
//__declspec(dllexport) void DetourAndErrorCheck(PVOID* pFunctionToDetour, PVOID pReplacement, const char* functionName) {

//	OutputDebugString(L"In function");
//	wchar_t debugString[256];
//	ZeroMemory(debugString, 256);
//	DetourTransactionBegin();
//	DetourUpdateThread(GetCurrentThread());
//	DetourAttach(pFunctionToDetour, pReplacement);
//	int errorCode = DetourTransactionCommit();
//	if(errorCode == NO_ERROR)
//	{
//		//sprintf_s(debugString, 255, "Address of %s: 0x%x\n"
//		//    "Address of replacement function: 0x%x\n"
//		//    "%s() was detoured successfully", functionName, pFunctionToDetour,
//		//    pReplacement, functionName);
//		OutputDebugString(debugString);
//	}
//	else
//	{
//		if(errorCode == ERROR_INVALID_DATA)
//			OutputDebugString(L"ERROR_INVALID_DATA returned");
//		else if(errorCode == ERROR_INVALID_OPERATION)
//			OutputDebugString(L"ERROR_INVALID_OPERATION returned");
//		else
//			OutputDebugString(L"Other error code returned");
//		//sprintf_s(debugString, 255, L"%s() was not able to be detoured\n"
//		//        L"Address of %s: 0x%x\n"
//		//        L"Address of replacement function: 0x%x", functionName,
//		//        functionName, pFunctionToDetour, pReplacement);
//		OutputDebugString(debugString);
//	}
//	delete [] debugString;

//}

