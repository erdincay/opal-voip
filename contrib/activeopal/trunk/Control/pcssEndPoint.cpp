/*
*
* pcssEndPoint.cpp
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

PCSSEndPoint::PCSSEndPoint(ActiveOpalManager & manager) : OpalPCSSEndPoint(manager), m_manager(manager)
{
}

PCSSEndPoint::~PCSSEndPoint()
{
}

PBoolean PCSSEndPoint::OnShowIncoming(const OpalPCSSConnection & connection)
{
	m_manager.m_opalControl->send_OnIncomingEvent();
	return TRUE;
}

PBoolean PCSSEndPoint::OnShowOutgoing(const OpalPCSSConnection & connection)
{
	m_manager.m_opalControl->send_OnRingingEvent();
	return TRUE;
}

BOOL PCSSEndPoint::Initialize()
{
	USES_CONVERSION;

    SetSoundChannelBufferDepth(10);
    
    SetSoundChannelPlayDevice (PString(W2A(m_manager.m_opalControl->m_SoundPlayerDevice)));
    SetSoundChannelRecordDevice (PString(W2A(m_manager.m_opalControl->m_SoundRecorderDevice)));

	return TRUE;
}
