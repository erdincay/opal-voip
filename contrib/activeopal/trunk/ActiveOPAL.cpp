/*
 * ActiveOPAL.cpp
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
#include "resource.h"


// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{AA9B2A43-2EFF-4A7C-BA2E-B0D520ADE85E}", 
         name = "ActiveOPAL", 
         helpstring = "ActiveOPAL 1.0 Type Library",
         resource_name = "IDR_ACTIVEOPAL") ]

class CActiveOPALModule
{
  public:
  // Override CAtlDllModuleT members
    class PWLibProcess : public PProcess 
    {
      PCLASSINFO(PWLibProcess, PProcess);

      void Main() { }
    } pwlibProcess;
};

