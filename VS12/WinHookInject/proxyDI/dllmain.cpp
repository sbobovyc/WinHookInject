// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "IDirectInputDevice8Hook.h"
#include "IDirectInput8Hook.h"

HRESULT (WINAPI *oldDirectInput8Create) (HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter) = NULL;

HRESULT WINAPI myDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter);
void InstallD3D8Hook();
void UninstallD3D8Hook();

#ifndef __cplusplus
extern "C" {
#endif

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	OutputDebugString(L"Starting up DLL");
	if (DetourIsHelperProcess()) {
		return TRUE;
	}
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InstallD3D8Hook();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		UninstallD3D8Hook();
		break;
	}
	return TRUE;
}

HRESULT WINAPI myDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
{
	HRESULT hr = oldDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	IDirectInput8 * pDInput = (IDirectInput8 *)*ppvOut;
	*ppvOut = new IDirectInput8Hook(pDInput);
	return hr;
}

void InstallD3D8Hook()
{
	if(!oldDirectInput8Create)
	{	
		//TODO add error handling to detours functions
		OutputDebugString(L"Hooking");	
		DetourRestoreAfterWith();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());		

		oldDirectInput8Create = (HRESULT(WINAPI *)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter))DetourFindFunction("dinput8.dll", "DirectInput8Create");
		DetourAttach(&(PVOID&)oldDirectInput8Create, (PBYTE)myDirectInput8Create);
		DetourTransactionCommit();
	}
}

void UninstallD3D8Hook()
{
	if(oldDirectInput8Create)
	{
		//TODO add error handling to detours functions
		OutputDebugString(L"Unhooking");	
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)oldDirectInput8Create, myDirectInput8Create);		
		DetourTransactionCommit();		
	}

}

#ifndef __cplusplus
}
#endif
