/*
 * contain.h
 *
 * Low level object and container definitions.
 *
 * Portable Windows Library
 *
 * Copyright (c) 1993-1998 Equivalence Pty. Ltd.
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
 * The Original Code is Portable Windows Library.
 *
 * The Initial Developer of the Original Code is Equivalence Pty. Ltd.
 *
 * Portions are Copyright (C) 1993 Free Software Foundation, Inc.
 * All Rights Reserved.
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: contain.h,v $
 * Revision 1.16  2003/09/17 01:18:03  csoutheren
 * Removed recursive include file system and removed all references
 * to deprecated coooperative threading support
 *
 * Revision 1.15  2002/06/06 09:27:26  robertj
 * Fixed PINDEX macros for being signed no (gcc3 port).
 * iAdded limits.h for Solaris compatibility.
 *
 * Revision 1.14  2002/06/05 12:29:15  craigs
 * Changes for gcc 3.1
 *
 * Revision 1.13  1999/05/01 11:29:19  robertj
 * Alpha linux port changes.
 *
 * Revision 1.12  1999/01/08 01:27:43  robertj
 * Moved include of pmachdep to earlier in inclusions.
 *
 * Revision 1.11  1998/11/30 22:06:35  robertj
 * New directory structure.
 *
 * Revision 1.10  1998/11/03 10:56:33  robertj
 * Removed unused extern for PErrorStream
 *
 * Revision 1.9  1998/09/24 04:11:31  robertj
 * Added open software license.
 *
 */

#include "pmachdep.h"
#include <unistd.h>
#include <ctype.h>
#include <limits.h>


///////////////////////////////////////////
//
//  define TRUE and FALSE for environments that don't have them
//

#ifndef	TRUE
#define	TRUE		1
#define	FALSE		0
#endif

///////////////////////////////////////////
//
//  define a macro for declaring classes so we can bolt
//  extra things to class declarations
//

#define	PEXPORT
#define	PSTATIC


///////////////////////////////////////////
//
// define some basic types and their limits
//

typedef int	           BOOL;
typedef unsigned char	   BYTE;	// 1 byte

typedef	signed short	   PInt16;	// 16 bit
typedef unsigned short	   WORD;

typedef	signed int         PInt32;	// 32 bit
typedef unsigned int 	   DWORD;

#ifndef P_NEEDS_INT64
typedef   signed long long int PInt64;
typedef unsigned long long int PUInt64;	// 64 bit
#endif

// Integer type that is same size as a pointer type.
#ifdef P_64BIT
typedef long INT;
#else
typedef int  INT;
#endif

typedef int PINDEX;
#define P_MAX_INDEX 		INT_MAX

inline PINDEX PABSINDEX(PINDEX idx) { return (idx < 0 ? -idx : idx)&P_MAX_INDEX; }
#define PASSERTINDEX(idx) PAssert((idx) >= 0, PInvalidArrayIndex)

