/*
 * mutex.h
 *
 * Mutual exclusion thread synchronisation class.
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
 * basis, WITHOUT WARRANTY OF ANY KIND, eitF ANY KIND, either express or implied. See
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
 * $Log: mutex.h,v $
 * Revision 1.11  2000/12/16 13:11:09  rogerh
 * Remote the 'public:' line. It is redundant as 'public:' is already
 * specified in the ../../mutex.h header file. Problem spotted following a
 * BeOS patch from Yuri Kiryanov <openh323@kiryanov.com>
 *
 * Revision 1.10  2000/12/15 13:20:17  rogerh
 * Fix typo
 *
 * Revision 1.9  2000/12/15 12:50:14  rogerh
 * Fix some BeOS problems with #if defines.
 * Requested by Yuri Kiryanov <openh323@kiryanov.com>
 *
 * Revision 1.8  2000/10/30 05:48:33  robertj
 * Added assert when get nested mutex.
 *
 * Revision 1.7  1999/09/23 06:52:16  robertj
 * Changed PSemaphore to use Posix semaphores.
 *
 * Revision 1.6  1999/09/02 11:56:35  robertj
 * Fixed problem with destroying PMutex that is already locked.
 *
 * Revision 1.5  1999/03/05 07:03:27  robertj
 * Some more BeOS port changes.
 *
 * Revision 1.4  1999/01/09 03:35:09  robertj
 * Improved efficiency of mutex to use pthread functions directly.
 *
 * Revision 1.3  1998/11/30 22:06:51  robertj
 * New directory structure.
 *
 * Revision 1.2  1998/09/24 04:11:41  robertj
 * Added open software license.
 *
 * Revision 1.1  1998/03/24 07:31:04  robertj
 * Initial revision
 *
 */


#ifndef _PMUTEX


///////////////////////////////////////////////////////////////////////////////
// PMutex

#include "../../mutex.h"
#if defined(P_PTHREADS) || defined(BE_THREADS)
    ~PMutex();
    virtual void Wait();
    virtual BOOL Wait(const PTimeInterval & timeout);
    virtual void Signal();
    virtual BOOL WillBlock() const;
  protected:

#if defined(P_PTHREADS)
    pthread_t ownerThreadId;
#endif

#if defined(P_PTHREADS) && defined(P_HAS_SEMAPHORES)
    pthread_mutex_t mutex;
#endif

#if defined(BE_THREADS)
    int32 benaphoreCount;
#endif

#endif
};


#endif
