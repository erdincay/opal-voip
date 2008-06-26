/*
*
* ActiveOpalControl.cpp : Implementation of CActiveOpalControl
*
* Copyright (C) 2007-2008 Telefonik (http://www.telefonik.com)
*
* Developer(s): Dennis White (dennis.white@telefonik.com)
*
* This file is part of ActiveOpal.

* ActiveOpal is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ActiveOpal is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with ActiveOpal.  If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "stdafx.h"
#include "ActiveOpalControl.h"
#include "activeOpalManager.h"


// CActiveOpalControl

STDMETHODIMP CActiveOpalControl::Initialize(void)
{
	m_opalManager = new ActiveOpalManager();
	
	if (FALSE == m_opalManager->Initialize(this))
		return S_FALSE;

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::UnInitialize(void)
{
	delete m_opalManager;
	m_opalManager = NULL;

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Register(void)
{
	USES_CONVERSION;
	
	if(FALSE == m_opalManager->Register(W2A(m_Registrar), W2A(m_UserName), W2A(m_Password)))
		return S_FALSE;

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::UnRegister(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Call(BSTR strCalledNumber)
{
	USES_CONVERSION;
	m_opalManager->StartCall(PString(W2A(strCalledNumber)));
	return S_OK;

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::RejectCall(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Hangup(void)
{
	m_opalManager->HangupCall();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::HoldCall(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::UnHoldCall(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::TransferCall(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::MuteInputDevice(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::MuteOutputDevice(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::UnMuteInputDevice(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::UnMuteOutputDevice(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::put_Registrar(BSTR newVal)
{
    m_Registrar = newVal;

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::put_UserName(BSTR newVal)
{
    m_UserName = newVal;

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::put_Password(BSTR newVal)
{
    m_Password = newVal;

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::get_PlayerSoundDevices(BSTR* pVal)
{
    USES_CONVERSION;
    string devnames;
    
	PStringArray devices = PSoundChannel::GetDeviceNames(PSoundChannel::Player);
    for (PINDEX i = 0 ; i < devices.GetSize(); i++) 
	{
        if (!devnames.empty())
			devnames += ';';
        
		devnames += (const char *)devices[i];
    }

    *pVal = A2BSTR(devnames.c_str());

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::get_RecorderSoundDevices(BSTR* pVal)
{
    USES_CONVERSION;
    string devnames;

	PStringArray devices = PSoundChannel::GetDeviceNames(PSoundChannel::Recorder);
    for (PINDEX i = 0 ; i < devices.GetSize(); i++) 
	{
        if (!devnames.empty())
			devnames += ';';

        devnames += (const char *)devices[i];
    }

    *pVal = A2BSTR(devnames.c_str());

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::put_SoundPlayerDevices(BSTR newVal)
{
    m_SoundPlayerDevice = newVal;

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::put_SoundRecorderDevices(BSTR newVal)
{
    m_SoundRecorderDevice = newVal;

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::put_StunServer(BSTR newVal)
{
	// set the stun server value and set it in use
    m_StunServer = newVal;
	m_useStunServer = TRUE;

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::put_AutoAnswer(VARIANT_BOOL newVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::put_AutoReRegistration(VARIANT_BOOL newVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

void CActiveOpalControl::send_OnRegisteringEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnRegisteringEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnRegisteredEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnRegisteredEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnUnRegisteringEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnUnRegisteringEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnUnRegisteredEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnUnRegisteredEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnUnRegistrationFailedEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnUnRegistrationFailedEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnRegistrationFailedEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnRegistrationFailedEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnReRegisteringEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnReRegisteringEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnReRegisterFailedEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnReRegisterFailedEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnReRegisteredEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnReRegisteredEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnCallClearedEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnCallClearedEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnCallingEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnCallingEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnRingingEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnRingingEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnHangingUpEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnHangingUpEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnAnsweringCallEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnAnsweringCallEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnInConversationEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnInConversationEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnCallDroppedEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnCallDroppedEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

void CActiveOpalControl::send_OnIncomingEvent()
{
	CComQIPtr<IActiveOpalControl> rpOpalControl;
	HRESULT hr;

	hr = AtlUnmarshalPtr(m_Stream, IID_IActiveOpalControl, (IUnknown**)&rpOpalControl);
	
	if (SUCCEEDED(hr))
		rpOpalControl->Trigger_OnIncomingEvent();
	
	rpOpalControl = (IActiveOpalControl*)NULL;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnRegisteringEvent(void)
{
	Fire_OnRegistering();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnRegisteredEvent(void)
{
	Fire_OnRegistered();
	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnUnRegisteringEvent(void)
{
	Fire_OnUnRegistering();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnUnRegisteredEvent(void)
{
	Fire_OnUnRegistered();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnUnRegistrationFailedEvent(void)
{
	Fire_OnUnRegistrationFailed();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnRegistrationFailedEvent(void)
{
	Fire_OnRegistrationFailed();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnReRegisteringEvent(void)
{
	Fire_OnReRegistering();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnReRegisterFailedEvent(void)
{
	Fire_OnReRegistrationFailed();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnReRegisteredEvent(void)
{
	// TODO: Add your implementation code here
	Fire_OnReRegistered();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnCallClearedEvent(void)
{
	Fire_OnCallCleared();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnCallingEvent(void)
{
	Fire_OnCalling();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnRingingEvent(void)
{
	Fire_OnRinging();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnHangingUpEvent(void)
{
	Fire_OnHangingUp();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnAnsweringCallEvent(void)
{
	Fire_OnAnsweringCall();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnInConversationEvent(void)
{
	Fire_OnInConversation();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnCallDroppedEvent(void)
{
	Fire_OnCallDropped();

	return S_OK;
}

STDMETHODIMP CActiveOpalControl::Trigger_OnIncomingEvent(void)
{
	Fire_OnIncoming();

	return S_OK;
}
