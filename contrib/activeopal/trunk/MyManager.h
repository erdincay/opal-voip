/*
 * MyManager.h
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

#pragma once


class MyManager; 
class CActiveOPALCtrl;


class MyPCSSEndPoint : public OpalPCSSEndPoint
{
  PCLASSINFO(MyPCSSEndPoint, OpalPCSSEndPoint);

public:
  MyPCSSEndPoint(MyManager & manager); 

  virtual PBoolean OnShowIncoming(const OpalPCSSConnection & connection);
  virtual PBoolean OnShowOutgoing(const OpalPCSSConnection & connection);

  PBoolean SetSoundDevice(PArgList & args, const char * optionName, PSoundChannel::Directions dir);

private:
  MyManager & m_manager;
  PString     incomingConnectionToken;
  PBoolean    autoAnswer;
};


class MySIPEndPoint : public SIPEndPoint
{
  PCLASSINFO(MySIPEndPoint, SIPEndPoint);
public:
  MySIPEndPoint (MyManager & manager);
  virtual void OnRegistered(const PString & aor, PBoolean wasRegistering);
  virtual void OnRegistrationFailed(const PString & aor, SIP_PDU::StatusCodes reason, PBoolean wasRegistering);

private:
  MyManager & m_manager;
};


class MyManager : public OpalManager
{
  PCLASSINFO(MyManager, OpalManager);
public:
  MyManager();
  ~MyManager();

  PBoolean Initialise(CActiveOPALCtrl *ctrl);
  void StartCall(PString CalledNumber);
  void HangupCall();
  bool Register(PString Registrar, PString UserName, PString Password);
  virtual void OnEstablishedCall(OpalCall & call);
  virtual void OnClearedCall(OpalCall & call);

protected: 
  void ListSpeedDials();

  void NewSpeedDial(const PString & ostr);
  void SendMessageToRemoteNode(const PString & ostr);
  void SendTone(const char tone);

  CActiveOPALCtrl       * ctrl1;
  PString            m_currentCallToken;
  MyPCSSEndPoint   * pcssEP;
  SIPEndPoint      * sipEP;
  PBoolean           pauseBeforeDialing;
  PString            srcEP;

  friend class MyPCSSEndPoint;
  friend class MySIPEndPoint;
};

