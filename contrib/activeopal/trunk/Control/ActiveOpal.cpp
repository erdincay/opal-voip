/*
*
* activeOpal.cpp
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
#include "resource.h"
#include "ActiveOpal.h"


class CActiveOpalModule : public CAtlDllModuleT< CActiveOpalModule >
{
public :
	DECLARE_LIBID(LIBID_ActiveOpalLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ACTIVEOPAL, "{0ADA4E88-3D76-47FE-A15C-FCEDE3BC7241}")
};

CActiveOpalModule _AtlModule;

// This is our pwlib processing thread that is required for everything to work
PDECLARE_PROCESS(PWLibProcess, PProcess, "ActiveOPAL VoIP", "ActiveX control for use with OPAL", 1, 0, ReleaseCode, 1)
static PWLibProcess pwlibProcess;
void PWLibProcess::Main()
{
}

#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;

	if(dwReason == DLL_PROCESS_DETACH)
	{
		// added this to assure that the pwlib thread gets shutdown (not sure if this is correct??)
		pwlibProcess.Terminate();
	}

	return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

