//----------------------------------------------------------
//
// VC:P Multiplayer Modification For GTA:VC
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include"stdafx.h"
#include "IDirectInputDevice8Hook.h"

IDirectInputDevice8Hook::IDirectInputDevice8Hook(IDirectInput8 * dinput, IDirectInputDevice8 * dinputdevice)
{
	m_pDI = dinput;
	m_pDIDevice = dinputdevice;
}

/*** IUnknown methods ***/
HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return m_pDIDevice->QueryInterface(riid, ppvObj);
}

ULONG STDMETHODCALLTYPE IDirectInputDevice8Hook::AddRef()
{
	return m_pDIDevice->AddRef();
}

ULONG STDMETHODCALLTYPE IDirectInputDevice8Hook::Release()
{
	ULONG uRet = m_pDIDevice->Release();

	if(uRet == 0)
		// If the reference count is 0 delete ourselves
		delete this;

	return uRet;
}

/*** IDirectInputDevice8A methods ***/
HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetCapabilities(LPDIDEVCAPS p0)
{
	return m_pDIDevice->GetCapabilities(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW p0, LPVOID p1, DWORD p2)
{
	return m_pDIDevice->EnumObjects(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetProperty(REFGUID rguid, LPDIPROPHEADER p1)
{
	return m_pDIDevice->GetProperty(rguid, p1);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SetProperty(REFGUID rguid, LPCDIPROPHEADER p1)
{
	return m_pDIDevice->SetProperty(rguid, p1);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::Acquire()
{
	return m_pDIDevice->Acquire();
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::Unacquire()
{
	return m_pDIDevice->Unacquire();
}	

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetDeviceState(DWORD p0, LPVOID p1)
{
	HRESULT hResult = m_pDIDevice->GetDeviceState(p0, p1);
	return hResult;
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetDeviceData(DWORD p0, LPDIDEVICEOBJECTDATA p1, LPDWORD p2, DWORD p3)
{
	HRESULT hResult = m_pDIDevice->GetDeviceData(p0, p1, p2, p3);
	return hResult;
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SetDataFormat(LPCDIDATAFORMAT p0)
{
	return m_pDIDevice->SetDataFormat(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SetEventNotification(HANDLE p0)
{
	return m_pDIDevice->SetEventNotification(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SetCooperativeLevel(HWND p0, DWORD p1)
{
	return m_pDIDevice->SetCooperativeLevel(p0, p1);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW p0, DWORD p1, DWORD p2)
{
	return m_pDIDevice->GetObjectInfo(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetDeviceInfo(LPDIDEVICEINSTANCEW p0)
{
	return m_pDIDevice->GetDeviceInfo(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::RunControlPanel(HWND p0, DWORD p1)
{
	return m_pDIDevice->RunControlPanel(p0, p1);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::Initialize(HINSTANCE p0, DWORD p1, REFGUID rguid)
{
	return m_pDIDevice->Initialize(p0, p1, rguid);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::CreateEffect(REFGUID rguid, LPCDIEFFECT p1, LPDIRECTINPUTEFFECT * p2, LPUNKNOWN p3)
{
	return m_pDIDevice->CreateEffect(rguid, p1, p2, p3);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::EnumEffects(LPDIENUMEFFECTSCALLBACKW p0, LPVOID p1, DWORD p2)
{
	return m_pDIDevice->EnumEffects(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetEffectInfo(LPDIEFFECTINFOW p0, REFGUID rguid)
{
	return m_pDIDevice->GetEffectInfo(p0, rguid);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetForceFeedbackState(LPDWORD p0)
{
	return m_pDIDevice->GetForceFeedbackState(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SendForceFeedbackCommand(DWORD p0)
{
	return m_pDIDevice->SendForceFeedbackCommand(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK p0, LPVOID p1, DWORD p2)
{
	return m_pDIDevice->EnumCreatedEffectObjects(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::Escape(LPDIEFFESCAPE p0)
{
	return m_pDIDevice->Escape(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::Poll()
{
	return m_pDIDevice->Poll();
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SendDeviceData(DWORD p0, LPCDIDEVICEOBJECTDATA p1, LPDWORD p2, DWORD p3)
{
	return m_pDIDevice->SendDeviceData(p0, p1, p2, p3);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::EnumEffectsInFile(LPCWSTR p0, LPDIENUMEFFECTSINFILECALLBACK p1, LPVOID p2, DWORD p3)
{
	return m_pDIDevice->EnumEffectsInFile(p0, p1, p2, p3);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::WriteEffectToFile(LPCWSTR p0, DWORD p2, LPDIFILEEFFECT p3, DWORD p4)
{
	return m_pDIDevice->WriteEffectToFile(p0, p2, p3, p4);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::BuildActionMap(LPDIACTIONFORMATW p0, LPCWSTR p1, DWORD p2)
{
	return m_pDIDevice->BuildActionMap(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SetActionMap(LPDIACTIONFORMATW p0, LPCWSTR p1, DWORD p2)
{
	return m_pDIDevice->SetActionMap(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERW p0)
{
	return m_pDIDevice->GetImageInfo(p0);
}
