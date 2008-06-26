/*
*
* ActiveOpalControl.h : Declaration of the CActiveOpalControl
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
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "ActiveOpal.h"
#include "_IActiveOpalControlEvents_CP.h"

#include "activeOpalEventManager.h"

class ActiveOpalManager;

#ifdef _WIN32_WCE
#error "ATL does not support HTML controls for Windows CE."
#endif

// CActiveOpalControl
class ATL_NO_VTABLE CActiveOpalControl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IActiveOpalControl, &IID_IActiveOpalControl, &LIBID_ActiveOpalLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CActiveOpalControl>,
	public IOleControlImpl<CActiveOpalControl>,
	public IOleObjectImpl<CActiveOpalControl>,
	public IOleInPlaceActiveObjectImpl<CActiveOpalControl>,
	public IViewObjectExImpl<CActiveOpalControl>,
	public IOleInPlaceObjectWindowlessImpl<CActiveOpalControl>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CActiveOpalControl>,
	public CProxy_IActiveOpalControlEvents<CActiveOpalControl>,
	public IPersistStorageImpl<CActiveOpalControl>,
	public ISpecifyPropertyPagesImpl<CActiveOpalControl>,
	public IQuickActivateImpl<CActiveOpalControl>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CActiveOpalControl>,
#endif
	public IProvideClassInfo2Impl<&CLSID_ActiveOpalControl, &__uuidof(_IActiveOpalControlEvents), &LIBID_ActiveOpalLib>,
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	public IObjectSafetyImpl<CActiveOpalControl, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
	public CComCoClass<CActiveOpalControl, &CLSID_ActiveOpalControl>,
	public CComControl<CActiveOpalControl>,
	public ActiveOpalEventManager
{
protected:
	IStream *m_Stream;

public:

	void send_OnRegisteringEvent();
	void send_OnRegisteredEvent();
	void send_OnUnRegisteringEvent();
	void send_OnUnRegisteredEvent();
	void send_OnUnRegistrationFailedEvent();
	void send_OnRegistrationFailedEvent();
 	void send_OnReRegisteringEvent();
	void send_OnReRegisterFailedEvent();
	void send_OnReRegisteredEvent();
	void send_OnCallClearedEvent();
	void send_OnCallingEvent();
	void send_OnRingingEvent();
	void send_OnHangingUpEvent();
	void send_OnAnsweringCallEvent();
	void send_OnInConversationEvent();
	void send_OnCallDroppedEvent();
	void send_OnIncomingEvent();

	CActiveOpalControl()
	{
		m_bWindowOnly = TRUE;
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_ACTIVEOPALCONTROL)


BEGIN_COM_MAP(CActiveOpalControl)
	COM_INTERFACE_ENTRY(IActiveOpalControl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
#endif
END_COM_MAP()

BEGIN_PROP_MAP(CActiveOpalControl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CActiveOpalControl)
	CONNECTION_POINT_ENTRY(__uuidof(_IActiveOpalControlEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CActiveOpalControl)
	CHAIN_MSG_MAP(CComControl<CActiveOpalControl>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] =
		{
			&IID_IActiveOpalControl,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IActiveOpalControl

	STDMETHOD(TranslateAccelerator)(LPMSG pMsg)
	{
		CComPtr<IOleInPlaceActiveObject> spIOleInPlaceActiveObject;

		HRESULT hr = m_spBrowser->QueryInterface(&spIOleInPlaceActiveObject);
		if (SUCCEEDED(hr))
			hr = spIOleInPlaceActiveObject->TranslateAccelerator(pMsg);
		if (hr != S_OK)
			hr = IOleInPlaceActiveObjectImpl<CActiveOpalControl>::TranslateAccelerator(pMsg);

		return hr;
	}
	CComPtr<IWebBrowser2> m_spBrowser;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		HRESULT hr;
		
		CComQIPtr<IActiveOpalControl> tOpal;
		hr = QueryInterface(IID_IActiveOpalControl, (void **)&tOpal);
		
		if (SUCCEEDED(hr))
		{
			hr = AtlMarshalPtrInProc(tOpal, IID_IActiveOpalControl, &m_Stream);
		}

		return S_OK;
	}

	void FinalRelease()
	{
		AtlFreeMarshalStream(m_Stream);
	}

	wchar_t* m_Registrar;
	wchar_t* m_UserName;
	wchar_t* m_Password;
	wchar_t *m_SoundPlayerDevice;
	wchar_t *m_SoundRecorderDevice;

	ActiveOpalManager * m_opalManager;
	
	BOOL	m_useStunServer;
	wchar_t *m_StunServer;

	STDMETHOD(Initialize)(void);
	STDMETHOD(UnInitialize)(void);
	STDMETHOD(Register)(void);
	STDMETHOD(UnRegister)(void);
	STDMETHOD(Call)(BSTR strCalledNumber);
	STDMETHOD(RejectCall)(void);
	STDMETHOD(Hangup)(void);
	STDMETHOD(HoldCall)(void);
	STDMETHOD(UnHoldCall)(void);
	STDMETHOD(TransferCall)(void);
	STDMETHOD(MuteInputDevice)(void);
	STDMETHOD(MuteOutputDevice)(void);
	STDMETHOD(UnMuteInputDevice)(void);
	STDMETHOD(UnMuteOutputDevice)(void);
	STDMETHOD(put_Registrar)(BSTR newVal);
	STDMETHOD(put_UserName)(BSTR newVal);
	STDMETHOD(put_Password)(BSTR newVal);
	STDMETHOD(get_PlayerSoundDevices)(BSTR* pVal);
	STDMETHOD(get_RecorderSoundDevices)(BSTR* pVal);
	STDMETHOD(put_SoundPlayerDevices)(BSTR newVal);
	STDMETHOD(put_SoundRecorderDevices)(BSTR newVal);
	STDMETHOD(put_StunServer)(BSTR newVal);
	STDMETHOD(put_AutoAnswer)(VARIANT_BOOL newVal);
	STDMETHOD(put_AutoReRegistration)(VARIANT_BOOL newVal);
	STDMETHOD(Trigger_OnRegisteringEvent)(void);
	STDMETHOD(Trigger_OnRegisteredEvent)(void);
	STDMETHOD(Trigger_OnUnRegisteringEvent)(void);
	STDMETHOD(Trigger_OnUnRegisteredEvent)(void);
	STDMETHOD(Trigger_OnUnRegistrationFailedEvent)(void);
	STDMETHOD(Trigger_OnRegistrationFailedEvent)(void);
	STDMETHOD(Trigger_OnReRegisteringEvent)(void);
	STDMETHOD(Trigger_OnReRegisterFailedEvent)(void);
	STDMETHOD(Trigger_OnReRegisteredEvent)(void);
	STDMETHOD(Trigger_OnCallClearedEvent)(void);
	STDMETHOD(Trigger_OnCallingEvent)(void);
	STDMETHOD(Trigger_OnRingingEvent)(void);
	STDMETHOD(Trigger_OnHangingUpEvent)(void);
	STDMETHOD(Trigger_OnAnsweringCallEvent)(void);
	STDMETHOD(Trigger_OnInConversationEvent)(void);
	STDMETHOD(Trigger_OnCallDroppedEvent)(void);
	STDMETHOD(Trigger_OnIncomingEvent)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(ActiveOpalControl), CActiveOpalControl)
