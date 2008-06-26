/*
*
* activeOpalGlobalInterfaceTable.h
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
#ifndef ACTIVEOPALGIT_H
#define ACTIVEOPALGIT_H

#pragma warning(disable:4530)	//Template memory warning
#pragma warning(disable:4786)	//Template memory warning

#include <map>
#include <comdef.h>
#include <atlcom.h>
using namespace std;

class CComDynamicUnkArray_GlobalInterfaceTable : public CComDynamicUnkArray
{
    private:
        IGlobalInterfaceTable*  GIT;

    public:

        CComDynamicUnkArray_GlobalInterfaceTable() : CComDynamicUnkArray()
        { 
            GIT = NULL;

            CoCreateInstance( CLSID_StdGlobalInterfaceTable, NULL, CLSCTX_INPROC_SERVER, 
                              __uuidof(IGlobalInterfaceTable), reinterpret_cast< void** >(&GIT) );
        }

	    ~CComDynamicUnkArray_GlobalInterfaceTable()
	    {
            clear();
            if( GIT != NULL )
            {
                GIT->Release();
            }
	    }

	    DWORD Add(IUnknown* pUnk);
	    BOOL Remove(DWORD dwCookie);

        CComPtr<IUnknown> GetAt(int nIndex)
	    {
            DWORD dwCookie = (DWORD)CComDynamicUnkArray::GetAt( nIndex );

            if( dwCookie == 0 )
			    return NULL;

            if( CookieMap.find( dwCookie ) == CookieMap.end() )
            {
    		    return (IUnknown*)dwCookie;
            }
            if( GIT != NULL )
            {
                CComPtr<IUnknown>   ppv;

                HRESULT hr = GIT->GetInterfaceFromGlobal(
                  CookieMap[dwCookie],              //Cookie identifying the desired global 
                                                    //interface and its object
                  __uuidof(IUnknown),               //IID of the registered global interface
                  reinterpret_cast< void** >(&ppv)  //Indirect pointer to the desired interface
                );
                if( hr == S_OK )
                {
                    return ppv;
                }
            }
		    return (IUnknown*)dwCookie;
	    }

	    void clear()
	    {
            CComDynamicUnkArray::clear();

            if( GIT != NULL )
            {
                map< DWORD, DWORD >::iterator iter;
                for( iter = CookieMap.begin(); iter != CookieMap.end(); ++iter )
                {
                    GIT->RevokeInterfaceFromGlobal(
                      iter->second              //Cookie that was returned from 
                                                //RegisterInterfaceInGlobal
                    );
                }
            }
            CookieMap.clear();
	    }
    protected:
        map< DWORD, DWORD > CookieMap;
};

inline DWORD CComDynamicUnkArray_GlobalInterfaceTable::Add(IUnknown* pUnk)
{
    DWORD Result = CComDynamicUnkArray::Add( pUnk );

    HRESULT hr;
    DWORD   pdwCookie = 0;
    if( GIT != NULL )
    {
        hr = GIT->RegisterInterfaceInGlobal(
          pUnk,                 //Pointer to interface of type riid of object 
                                //containing global interface
          __uuidof(IUnknown),   //IID of the interface to be registered
          &pdwCookie            //Supplies a pointer to the cookie that provides 
                                //a caller in another apartment access to the 
                                //interface pointer
        );
    }
    if( hr == S_OK )
    {
        CookieMap[Result] = pdwCookie;
    }

	return Result;
}

inline BOOL CComDynamicUnkArray_GlobalInterfaceTable::Remove(DWORD dwCookie)
{
    BOOL Result = CComDynamicUnkArray::Remove( dwCookie );

    if( GIT != NULL )
    {
        if( CookieMap.find( dwCookie ) != CookieMap.end() )
        {
            GIT->RevokeInterfaceFromGlobal(
              CookieMap[dwCookie]   //Cookie that was returned from 
                                    //RegisterInterfaceInGlobal
            );
            CookieMap.erase(dwCookie);
        }
    }
    return Result;
}

#endif // ACTIVEOPALGIT_H