/*
 * $Id: ptlib.h,v 1.5 1994/08/22 00:46:48 robertj Exp $
 *
 * Portable Windows Library
 *
 * User Interface Classes Interface Declarations
 *
 * Copyright 1993 by Robert Jongbloed and Craig Southeren
 *
 * $Log: ptlib.h,v $
 * Revision 1.5  1994/08/22 00:46:48  robertj
 * Added pragma fro GNU C++ compiler.
 *
 * Revision 1.4  1994/07/25  03:36:03  robertj
 * Added sockets to common, normalising to same comment standard.
 *
 * Revision 1.3  1994/07/21  12:17:41  robertj
 * Sockets.
 *
 * Revision 1.2  1994/06/25  12:27:39  robertj
 * *** empty log message ***
 *
 * Revision 1.1  1994/04/01  14:38:42  robertj
 * Initial revision
 *
 */

#ifndef _PTLIB_H
#define _PTLIB_H

#ifdef __GNU__
#pragma interface
#endif


#include <contain.h>


///////////////////////////////////////////////////////////////////////////////
// PTime

#include <ptime.h>


///////////////////////////////////////////////////////////////////////////////
// PTimeInterval

#include <timeint.h>


///////////////////////////////////////////////////////////////////////////////
// PTimer

#include <timer.h>


///////////////////////////////////////////////////////////////////////////////
// PDirectory

#include <pdirect.h>


///////////////////////////////////////////////////////////////////////////////
// PChannel

#include <channel.h>


///////////////////////////////////////////////////////////////////////////////
// PSerialChannel

#include <serchan.h>


///////////////////////////////////////////////////////////////////////////////
// PModem

#include <modem.h>


///////////////////////////////////////////////////////////////////////////////
// PFilePath

#include <filepath.h>


///////////////////////////////////////////////////////////////////////////////
// PFile

#include <file.h>


///////////////////////////////////////////////////////////////////////////////
// PTextFile

#include <textfile.h>


///////////////////////////////////////////////////////////////////////////////
// PStructuredFile

#include <sfile.h>


///////////////////////////////////////////////////////////////////////////////
// PConfig

#include <config.h>


///////////////////////////////////////////////////////////////////////////////
// PArgList

#include <args.h>


///////////////////////////////////////////////////////////////////////////////
// PThread

#include <thread.h>


///////////////////////////////////////////////////////////////////////////////
// PProcess

#include <process.h>


///////////////////////////////////////////////////////////////////////////////


#if defined(P_USE_INLINES)
#include <ptlib.inl>
#include <osutil.inl>
#endif


#endif // _PTLIB_H


// End Of File ///////////////////////////////////////////////////////////////
