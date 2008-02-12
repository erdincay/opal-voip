/*
 * ActiveOPALCtrl.h
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
#include "resource.h"       // main symbols
#include <atlctl.h>

class MyManager;


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IActiveOPALCtrl
[
	object,
	uuid(414BCCE9-B5DB-4DB7-933E-57DB137D939D),
	dual,
	helpstring("IActiveOPALCtrl Interface"),
	pointer_default(unique)
]
__interface IActiveOPALCtrl : public IDispatch
{
	[propput, id(1), helpstring("property Registrar")] HRESULT Registrar([in] BSTR newVal);
	[propput, id(2), helpstring("property UserName")] HRESULT UserName([in] BSTR newVal);
	[propput, id(3), helpstring("property Password")] HRESULT Password([in] BSTR newVal);
	[id(4), helpstring("method Register")] HRESULT Register(void);
	[id(5), helpstring("method Initialize")] HRESULT Initialize(void);
	[propget, id(6), helpstring("property OutputResponse")] HRESULT OutputResponse([out, retval] BSTR* pVal);
	[propput, id(7), helpstring("property CalledNumber")] HRESULT CalledNumber([in] BSTR newVal);
	[id(8), helpstring("method Call")] HRESULT Call(void);
	[id(9), helpstring("method HangupCall")] HRESULT HangupCall(void);
	[propget, id(10), helpstring("property GetPlayerSoundDevices")] HRESULT GetPlayerSoundDevices([out, retval] BSTR* pVal);
	[propget, id(11), helpstring("property GetRecorderSoundDevices")] HRESULT GetRecorderSoundDevices([out, retval] BSTR* pVal);
	[propput, id(12), helpstring("property SetSoundPlayerDevice")] HRESULT SetSoundPlayerDevice([in] BSTR newVal);
	[propput, id(13), helpstring("property SetSoundRecorderDevice")] HRESULT SetSoundRecorderDevice([in] BSTR newVal);
};


// _IActiveOPALCtrlEvents
[
	uuid("B8EB4EDA-D3B9-4293-B3E7-98990A39F1A2"),
	dispinterface,
	helpstring("_IActiveOPALCtrlEvents Interface")
]
__interface _IActiveOPALCtrlEvents
{
};

// CActiveOPALCtrl
[
	coclass,
	control,
	default(IActiveOPALCtrl, _IActiveOPALCtrlEvents),
	threading(apartment),
	vi_progid("ActiveOPAL.ActiveOPALCtrl"),
	progid("ActiveOPAL.ActiveOPALCtrl.1"),
	version(1.0),
	uuid("4B70E933-5B67-47DB-BADF-D86500FAC381"),
	helpstring("ActiveOPALCtrl Class"),
	event_source(com),
	support_error_info(IActiveOPALCtrl),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CActiveOPALCtrl :
	public IActiveOPALCtrl,
	public IPersistStreamInitImpl<CActiveOPALCtrl>,
	public IOleControlImpl<CActiveOPALCtrl>,
	public IOleObjectImpl<CActiveOPALCtrl>,
	public IOleInPlaceActiveObjectImpl<CActiveOPALCtrl>,
	public IViewObjectExImpl<CActiveOPALCtrl>,
	public IOleInPlaceObjectWindowlessImpl<CActiveOPALCtrl>,
	public IPersistStorageImpl<CActiveOPALCtrl>,
	public ISpecifyPropertyPagesImpl<CActiveOPALCtrl>,
	public IQuickActivateImpl<CActiveOPALCtrl>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CActiveOPALCtrl>,
#endif
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	public IObjectSafetyImpl<CActiveOPALCtrl, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
	public CComControl<CActiveOPALCtrl>
{
public:


	CActiveOPALCtrl()
		: m_Registrar(NULL)
		, m_UserName(NULL)
		, m_Password(NULL)
		, m_OutputResponse(NULL)
		, m_SoundPlayerDevice(NULL)
		, m_SoundRecorderDevice(NULL)
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CActiveOPALCtrl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CActiveOPALCtrl)
	CHAIN_MSG_MAP(CComControl<CActiveOPALCtrl>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	__event __interface _IActiveOPALCtrlEvents;
// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IActiveOPALCtrl
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
		RECT& rc = *(RECT*)di.prcBounds;
		// Set Clip region to the rectangle specified by di.prcBounds
		HRGN hRgnOld = NULL;
		if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
			hRgnOld = NULL;
		bool bSelectOldRgn = false;

		HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

		if (hRgnNew != NULL)
		{
			bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
		}

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 8.0 : ActiveOPALCtrl");
#ifndef _WIN32_WCE
		TextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			pszText,
			lstrlen(pszText));
#else
		ExtTextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			ETO_OPAQUE,
			NULL,
			pszText,
			ATL::lstrlen(pszText),
			NULL);
#endif

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);
		
		return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		delete m_pMyManager;
		m_pMyManager = NULL;
	}
public:

	void DoProcess(wchar_t* msg);
	
	
public:
	STDMETHOD(put_Registrar)(BSTR newVal);
public:
	STDMETHOD(put_UserName)(BSTR newVal);
public:
	STDMETHOD(put_Password)(BSTR newVal);
public:
	STDMETHOD(Register)(void);
	wchar_t* m_Registrar;
	wchar_t* m_UserName;
	wchar_t* m_Password;
	wchar_t* m_CalledNumber;
	MyManager *m_pMyManager;
public:
	STDMETHOD(Initialize)(void);
public:
	STDMETHOD(get_OutputResponse)(BSTR* pVal);
public:
	wchar_t* m_OutputResponse;
public:
	STDMETHOD(put_CalledNumber)(BSTR newVal);
public:
	STDMETHOD(Call)(void);
public:
	STDMETHOD(HangupCall)(void);

public:
	STDMETHOD(get_GetPlayerSoundDevices)(BSTR* pVal);
public:
	STDMETHOD(get_GetRecorderSoundDevices)(BSTR* pVal);
public:
	STDMETHOD(put_SetSoundPlayerDevice)(BSTR newVal);
public:
	wchar_t *m_SoundPlayerDevice;
public:
	wchar_t *m_SoundRecorderDevice;
public:
	STDMETHOD(put_SetSoundRecorderDevice)(BSTR newVal);
};

