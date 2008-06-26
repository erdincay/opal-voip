/*
*
* sipEndPoint.cpp
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

SipEndPoint::SipEndPoint(ActiveOpalManager &manager) : SIPEndPoint(manager), m_manager(manager)
{
}

SipEndPoint::~SipEndPoint()
{
}

void SipEndPoint::OnRegistered(const PString &aor, BOOL wasRegistering)
{
	if(wasRegistering == TRUE)
	{
		m_manager.m_opalControl->send_OnRegisteredEvent();
	}
	else
	{
		m_manager.m_opalControl->send_OnRegisteredEvent();
	}
}

void SipEndPoint::OnRegistrationFailed(const PString &aor, SIP_PDU::StatusCodes reason, BOOL wasRegistering)
{
	m_manager.m_opalControl->send_OnRegistrationFailedEvent();
}

BOOL SipEndPoint::Initialize()
{
    SetMIMEForm(false);

    SetRetryTimeouts(10000, 30000);
    SetMaxRetries (2);

	StartListener("");
    
	return TRUE;
}
