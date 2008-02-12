/*
 * ActiveOPALCtrl.cpp
 *
 * Active X control for OPAL
 *
 * Open Phone Abstraction Library (OPAL)
 *
 * Copyright (c) 2008 Ahmad Zia
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Open Phone Abstraction Library.
 *
 * The Initial Developer of the Original Code is Ahmad Zia
 *
 * Contributor(s): ______________________________________.
 *
 * $Revision$
 * $Author$
 * $Date$
 */

#include "stdafx.h"
#include "ActiveOPALCtrl.h"
#include "MyManager.h"
#include "comdef.h"


// CActiveOPALCtrl
void CActiveOPALCtrl::DoProcess(wchar_t* msg)
{
    m_OutputResponse = msg;
}


STDMETHODIMP CActiveOPALCtrl::put_Registrar(BSTR newVal)
{
    m_Registrar = newVal;
    return S_OK;
}


STDMETHODIMP CActiveOPALCtrl::put_UserName(BSTR newVal)
{
    m_UserName = newVal;
    return S_OK;
}


STDMETHODIMP CActiveOPALCtrl::put_Password(BSTR newVal)
{
    m_Password = newVal;
    return S_OK;
}


STDMETHODIMP CActiveOPALCtrl::Register(void)
{
    m_pMyManager->Register(m_Registrar, m_UserName, m_Password);
    return S_OK;
}


STDMETHODIMP CActiveOPALCtrl::Initialize(void)
{
#ifdef _DEBUG
    PTrace::Initialise(5, "c:\\trace.txt",PTrace::Timestamp|PTrace::Thread|PTrace::FileAndLine);
#endif

    m_pMyManager = new MyManager();
    m_pMyManager->Initialise(this);
    return S_OK;
}

STDMETHODIMP CActiveOPALCtrl::get_OutputResponse(BSTR* pVal)
{
    *pVal = SysAllocString(m_OutputResponse);
    return S_OK;
}

STDMETHODIMP CActiveOPALCtrl::put_CalledNumber(BSTR newVal)
{
    m_CalledNumber = newVal;
    return S_OK;
}


STDMETHODIMP CActiveOPALCtrl::Call(void)
{
    m_pMyManager->StartCall(PString(m_CalledNumber));
    return S_OK;
}


STDMETHODIMP CActiveOPALCtrl::HangupCall(void)
{
    m_pMyManager->HangupCall();
    return S_OK;
}


STDMETHODIMP CActiveOPALCtrl::get_GetPlayerSoundDevices(BSTR* pVal)
{
    string devnames;
    PStringArray devices = PSoundChannel::GetDeviceNames(PSoundChannel::Player);
    for (PINDEX i = 0 ; i < devices.GetSize(); i++) {
        if (!devnames.empty())
          devnames += ';';
        devnames += (const char *)devices[i];
    }

    USES_CONVERSION;
    *pVal = A2BSTR(devnames.c_str());
    return S_OK;
}


STDMETHODIMP CActiveOPALCtrl::get_GetRecorderSoundDevices(BSTR* pVal)
{
    string devnames;
    PStringArray devices = PSoundChannel::GetDeviceNames(PSoundChannel::Recorder);
    for (PINDEX i = 0 ; i < devices.GetSize(); i++) {
        if (!devnames.empty())
          devnames += ';';
        devnames += (const char *)devices[i];
    }

    USES_CONVERSION;
    *pVal = A2BSTR(devnames.c_str());
    return S_OK;
}


STDMETHODIMP CActiveOPALCtrl::put_SetSoundPlayerDevice(BSTR newVal)
{
    m_SoundPlayerDevice = newVal;
    return S_OK;
}


STDMETHODIMP CActiveOPALCtrl::put_SetSoundRecorderDevice(BSTR newVal)
{
    m_SoundRecorderDevice = newVal;
    return S_OK;
}
