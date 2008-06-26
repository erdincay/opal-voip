/*
*
* activeOpalManager.h
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
#pragma once

class ActiveOpalManager;
class CActiveOpalControl;

class PCSSEndPoint : public OpalPCSSEndPoint
{
	PCLASSINFO(PCSSEndPoint, OpalPCSSEndPoint);
public:
	PCSSEndPoint(ActiveOpalManager & manager);
	~PCSSEndPoint();

	virtual PBoolean OnShowIncoming(const OpalPCSSConnection & connection);
	virtual PBoolean OnShowOutgoing(const OpalPCSSConnection & connection);

	BOOL Initialize();

private:
	ActiveOpalManager & m_manager;
};

class SipEndPoint : public SIPEndPoint
{
	PCLASSINFO(SipEndPoint, SIPEndPoint);
public:
	SipEndPoint(ActiveOpalManager & manager);
	~SipEndPoint();

	virtual void OnRegistered(const PString & aor, BOOL wasRegistering);
	virtual void OnRegistrationFailed(const PString & aor, SIP_PDU::StatusCodes reason, BOOL wasRegistering);

	BOOL Initialize();

private:
	ActiveOpalManager & m_manager;
};

class ActiveOpalManager : public OpalManager
{
	PCLASSINFO(ActiveOpalManager, OpalManager);
public:
	ActiveOpalManager();
	~ActiveOpalManager();

	BOOL Initialize(CActiveOpalControl *ctrl);
	BOOL Register(PString Registrar, PString UserName, PString Password);

	void StartCall(PString CalledNumber);
	void HangupCall();
	
	virtual void OnEstablishedCall(OpalCall & call);
	virtual void OnClearedCall(OpalCall & call);

protected: 
	CActiveOpalControl *	m_opalControl;
	PString					m_currentCallToken;
	PCSSEndPoint *			pcssEP;
	SipEndPoint *			sipEP;

	friend class PCSSEndPoint;
	friend class SipEndPoint;
};
