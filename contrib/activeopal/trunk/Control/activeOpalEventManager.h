/*
*
* activeOpalEventManager.h
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
#ifndef ACTIVEOPAL_EVENTMANAGER_H
#define ACTIVEOPAL_EVENTMANAGER_H

class ActiveOpalEventManager
{
public:
	ActiveOpalEventManager() {};

	virtual void send_OnRegisteringEvent() {};
	virtual void send_OnRegisteredEvent() {};
	virtual void send_OnUnRegisteringEvent() {};
	virtual void send_OnUnRegisteredEvent() {};
	virtual void send_OnUnRegistrationFailedEvent() {};
	virtual void send_OnRegistrationFailedEvent() {};
 	virtual void send_OnReRegisteringEvent() {};
	virtual void send_OnReRegisterFailedEvent() {};
	virtual void send_OnReRegisteredEvent() {};
 
	virtual void send_OnCallClearedEvent() {};

	virtual void send_OnCallingEvent() {};
	virtual void send_OnRingingEvent() {};

	virtual void send_OnHangingUpEvent() {};

	virtual void send_OnAnsweringCallEvent() {};

	virtual void send_OnInConversationEvent() {};

	virtual void send_OnCallDroppedEvent() {};

	virtual void send_OnIncomingEvent() {};
};

#endif // ACTIVEOPAL_EVENTMANAGER_H