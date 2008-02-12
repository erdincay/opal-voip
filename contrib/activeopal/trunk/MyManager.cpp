/*
 * MyManager.cpp
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
#include "MyManager.h"
#include "ActiveOPALctrl.h"


MyManager::MyManager()
{
  
}

MyManager::~MyManager()
{

}

PBoolean MyManager::Initialise(CActiveOPALCtrl *ctrl)
{
    ctrl1 = ctrl;
    autoStartTransmitVideo = PFalse;
    autoStartReceiveVideo = PFalse;

    SetAudioJitterDelay(50, 250);

    pcssEP = new MyPCSSEndPoint(*this);
    pcssEP->SetSoundChannelBufferDepth(10);
    
    pcssEP->SetSoundChannelPlayDevice (PString(ctrl->m_SoundPlayerDevice));
    pcssEP->SetSoundChannelRecordDevice (PString(ctrl->m_SoundRecorderDevice));

    silenceDetectParams.m_mode = OpalSilenceDetector::NoSilenceDetection;

    unsigned portbase = 0;
    unsigned portmax  = 0;

    SetTCPPorts  (portbase, portmax);
    SetUDPPorts  (portbase, portmax);
    SetRtpIpPorts(5000, 5199); 

    unsigned tos = 176 ;

    sipEP = new MySIPEndPoint(*this);
    sipEP->SetNATBindingTimeout (PTimeInterval (0, 20));
    sipEP->SetDefaultLocalPartyName(PString("jaffar"));
    sipEP->SetRetryTimeouts (10000, 15000);  
    sipEP->SetMaxRetries (2);
    
    sipEP->StartListener("");

    PStringArray r1 = "*iLBC*";
    PStringArray r2 = "*Speex*";
    //PStringArray r3 = "*G.729*";
    PStringArray r4 = "RFC4175*";
    //PStringArray r5 = "G.726*";
    PStringArray r6 = "G.711*";

    //SetMediaFormatMask(r1);
    //SetMediaFormatMask(r2);
    //SetMediaFormatMask(r3);
    //SetMediaFormatMask(r4);
    //SetMediaFormatMask(r5);
    //SetMediaFormatMask(r6);
    SetMediaFormatOrder(r6);
    if (sipEP != NULL)
    {
      AddRouteEntry("pots:.*\\*.*\\*.* = sip:<dn2ip>");
      AddRouteEntry("pots:.*           = sip:<da>");
      AddRouteEntry("pc:.*             = sip:<da>");
    }

    if(pcssEP != NULL)
        AddRouteEntry("sip:.*  = pc:<da>");        


  return PTrue;
}


void MyManager::StartCall(PString CalledNumber)
{
    ctrl1->DoProcess(_T("MSG-CALLING"));
    PString str = "sip:";
    str += CalledNumber + "@" + PString(ctrl1->m_Registrar);
    SetUpCall("pc:*", str, m_currentCallToken);
}

void MyManager::HangupCall()
{
    ctrl1->DoProcess(_T("MSG-HANGINGUP"));
  PSafePtr<OpalCall> call = FindCallWithLock(m_currentCallToken);
  if (call != NULL)
  {
    call->Clear();
    m_currentCallToken = PString();
  }
}


void MyManager::OnEstablishedCall(OpalCall & call)
{
    
  m_currentCallToken = call.GetToken();
  ctrl1->DoProcess(_T("MSG-INCONVERSATION"));
  
}

void MyManager::OnClearedCall(OpalCall & call)
{
    
  if (m_currentCallToken == call.GetToken())
    m_currentCallToken = PString();

  PString remoteName = '"' + call.GetPartyB() + '"';
  ctrl1->DoProcess(_T("MSG-CALL-CLEARED"));
  
  OpalManager::OnClearedCall(call);
}


MyPCSSEndPoint::MyPCSSEndPoint(MyManager & mgr)
  : OpalPCSSEndPoint(mgr), m_manager(mgr)
{
}


PBoolean MyPCSSEndPoint::OnShowIncoming(const OpalPCSSConnection & connection)
{
  return PTrue;
}


PBoolean MyPCSSEndPoint::OnShowOutgoing(const OpalPCSSConnection & connection)
{
  m_manager.ctrl1->DoProcess(_T("MSG-RINGING"));    
  return PTrue;
}


PBoolean MyPCSSEndPoint::SetSoundDevice(PArgList & args,
                                    const char * optionName,
                                    PSoundChannel::Directions dir)
{
  return PFalse;
}

bool MyManager::Register(PString Registrar, PString UserName, PString Password)
{
    ctrl1->DoProcess(_T("MSG-REGISTERING"));    
    sipEP->SetRegistrarTimeToLive(PTimeInterval(0, 3600));
    return sipEP->Register(Registrar, (PString)UserName, (PString)UserName, (PString)Password, (PString)Registrar);
}

MySIPEndPoint::MySIPEndPoint
 (MyManager & manager):
SIPEndPoint(manager),m_manager(manager)
{
}

void MySIPEndPoint::OnRegistered(const PString & aor, PBoolean wasRegistering)
{
    if(wasRegistering)
    {
        m_manager.ctrl1->DoProcess(_T("MSG-REGISTERED"));
    }
    else
        m_manager.ctrl1->DoProcess(_T("MSG-UNREGISTERED"));
}

void MySIPEndPoint::OnRegistrationFailed(const PString & aor, SIP_PDU::StatusCodes reason, PBoolean wasRegistering)
{
    m_manager.ctrl1->DoProcess(_T("MSG-REGISTRATION-FAILED"));
}
