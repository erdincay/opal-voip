/*
*
* activeOpalManager.cpp
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
#include "activeOpalManager.h"
#include "activeOpalControl.h"

ActiveOpalManager::ActiveOpalManager()
{
}

ActiveOpalManager::~ActiveOpalManager()
{
}

BOOL ActiveOpalManager::Initialize(CActiveOpalControl *ctrl)
{
	USES_CONVERSION;

	m_opalControl = ctrl;

    autoStartTransmitVideo = FALSE;
    autoStartReceiveVideo = FALSE;
	
	if (TRUE == m_opalControl->m_useStunServer)
		SetSTUNServer(PString(W2A(m_opalControl->m_StunServer)));

	OpalMediaFormatList allMediaFormats;

    SetAudioJitterDelay(50, 250);

    pcssEP = new PCSSEndPoint(*this);
	if (FALSE == pcssEP->Initialize())
		return FALSE;

    silenceDetectParams.m_mode = OpalSilenceDetector::NoSilenceDetection;

    unsigned portbase = 0;
    unsigned portmax  = 0;

    SetTCPPorts  (portbase, portmax);
    SetUDPPorts  (portbase, portmax);
    SetRtpIpPorts(5000, 5199); 

    sipEP = new SipEndPoint(*this);
	if (FALSE == sipEP->Initialize())
		return FALSE;

	allMediaFormats += pcssEP->GetMediaFormats();
	
	allMediaFormats = OpalTranscoder::GetPossibleFormats(allMediaFormats); // Add transcoders
	for (PINDEX i = 0; i < allMediaFormats.GetSize(); i++) 
	{
		if (!allMediaFormats[i].IsTransportable())
			allMediaFormats.RemoveAt(i--); // Don't show media formats that are not used over the wire
	}

	allMediaFormats.Remove(GetMediaFormatMask());
	allMediaFormats.Reorder(GetMediaFormatOrder());

    if (sipEP != NULL)
    {
		AddRouteEntry("pots:.*\\*.*\\*.* = sip:<dn2ip>");
		AddRouteEntry("pots:.*           = sip:<da>");
		AddRouteEntry("pc:.*             = sip:<da>");
    }

    if(pcssEP != NULL)
	{
        AddRouteEntry("sip:.*  = pc:<da>");        
	}

	return TRUE;
}

BOOL ActiveOpalManager::Register(PString Registrar, PString UserName, PString Password)
{
	m_opalControl->send_OnRegisteringEvent();
	
	sipEP->SetRegistrarTimeToLive(PTimeInterval(0, 3600));
	return sipEP->Register(Registrar, UserName, UserName, Password, Registrar);
}

void ActiveOpalManager::StartCall(PString CalledNumber)
{
	USES_CONVERSION;
    
	// build the call string
	PString str = "sip:";
    str += CalledNumber + "@" + PString(W2A(m_opalControl->m_Registrar));
    
	if (TRUE == SetUpCall("pc:*", str, m_currentCallToken))
		m_opalControl->send_OnCallingEvent();
}

void ActiveOpalManager::HangupCall()
{
	// using the safe pointer releases from having to manage the lock
	PSafePtr<OpalCall> currentCall = FindCallWithLock(m_currentCallToken);

	if (NULL != currentCall)
	{
		currentCall->Clear();
		m_currentCallToken = PString();
		
		// fire the hangup event since it really happened
		m_opalControl->send_OnHangingUpEvent();
	}
}

void ActiveOpalManager::OnEstablishedCall(OpalCall &call)
{
	m_currentCallToken = call.GetToken();
	m_opalControl->send_OnInConversationEvent();
}

void ActiveOpalManager::OnClearedCall(OpalCall &call)
{
	if(m_currentCallToken == call.GetToken())
		m_currentCallToken = PString();

	m_opalControl->send_OnCallClearedEvent();

	OpalManager::OnClearedCall(call);
}
