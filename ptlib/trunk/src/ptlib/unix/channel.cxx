/*
 * channel.cxx
 *
 * I/O channel classes implementation
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
 * $Log: channel.cxx,v $
 * Revision 1.31  2001/09/10 03:03:36  robertj
 * Major change to fix problem with error codes being corrupted in a
 *   PChannel when have simultaneous reads and writes in threads.
 *
 * Revision 1.30  2001/08/11 15:38:43  rogerh
 * Add Mac OS Carbon changes from John Woods <jfw@jfwhome.funhouse.com>
 *
 * Revision 1.29  2001/06/30 06:59:07  yurik
 * Jac Goudsmit from Be submit these changes 6/28. Implemented by Yuri Kiryanov
 *
 * Revision 1.28  2001/03/20 06:44:25  robertj
 * Lots of changes to fix the problems with terminating threads that are I/O
 *   blocked, especially when doing orderly shutdown of service via SIGTERM.
 *
 * Revision 1.27  2000/12/05 08:24:50  craigs
 * Fixed problem with EINTR causing havoc
 *
 * Revision 1.26  2000/05/15 23:33:06  craigs
 * Fixed problem where lastReadCount was not zeroed if no read occurred
 *
 * Revision 1.25  1999/03/02 05:41:59  robertj
 * More BeOS changes
 *
 * Revision 1.24  1999/02/22 13:26:53  robertj
 * BeOS port changes.
 *
 * Revision 1.23  1998/11/30 21:51:30  robertj
 * New directory structure.
 *
 * Revision 1.22  1998/10/16 02:03:18  robertj
 * Fixed error message output to include number on unknown errors.
 *
 * Revision 1.21  1998/10/16 01:15:38  craigs
 * Added Yield to help with cooperative multithreading.
 *
 * Revision 1.20  1998/10/11 02:23:16  craigs
 * Fixed problem with socket writes not correctly detecting EOF
 *
 * Revision 1.19  1998/09/24 04:12:09  robertj
 * Added open software license.
 *
 * Revision 1.18  1998/08/27 01:06:30  robertj
 * Fixed very strange link error with GNU C library v6.
 *
 * Revision 1.17  1998/05/25 10:03:26  robertj
 * Fixed problem with socket/channel close and blocked threads.
 *
 * Revision 1.16  1998/03/26 05:01:12  robertj
 * Added PMutex and PSyncPoint classes.
 *
 * Revision 1.15  1998/01/03 22:35:04  craigs
 * Added PThread support
 *
 * Revision 1.14  1997/02/14 09:18:36  craigs
 * Changed for PProcess::Current being a reference rather that a ptr
 *
 * Revision 1.13  1996/11/03 04:35:32  craigs
 * Added PSocket::Read to fix recv/read problem
 *
 * Revision 1.12  1996/09/21 05:38:28  craigs
 * Added indchan pragma
 *
 * Revision 1.11  1996/08/03 12:04:28  craigs
 * Fixed problem with PChannel::Write terminating early
 * Changed for new PChannel error reporting functions
 *
 * Revision 1.10  1996/05/25 06:06:33  craigs
 * Sun4 fixes and updated for gcc 2.7.2
 *
 * Revision 1.9  1996/05/03 13:11:35  craigs
 * More Sun4 fixes
 *
 * Revision 1.8  1996/05/02 12:01:23  craigs
 * More Sun4 fixes
 *
 * Revision 1.7  1996/04/15 10:49:11  craigs
 * Last build prior to release of MibMaster v1.0
 *
 * Revision 1.6  1996/01/26 11:09:42  craigs
 * Fixed problem with blocking accepts and incorrect socket errors
 *
 * Revision 1.5  1995/10/15 12:56:54  craigs
 * Multiple updates - split channel implementation into multiple files
 *
 * Revision 1.4  1995/07/09 00:35:43  craigs
 * Latest and greatest omnibus change
 *
 * Revision 1.3  1995/02/15 20:28:14  craigs
 * Removed sleep after pipe channel open
 *
// Revision 1.2  1995/01/23  22:58:01  craigs
// Changes for HPUX and Sun 4
//
 */

#pragma implementation "channel.h"
#pragma implementation "indchan.h"

#include <ptlib.h>
#include <sys/ioctl.h>


#include "../common/pchannel.cxx"


#ifdef P_NEED_IOSTREAM_MUTEX
static PMutex iostreamMutex;
#define IOSTREAM_MUTEX_WAIT()   iostreamMutex.Wait();
#define IOSTREAM_MUTEX_SIGNAL() iostreamMutex.Signal();
#else
#define IOSTREAM_MUTEX_WAIT()
#define IOSTREAM_MUTEX_SIGNAL()
#endif


ios::ios(const ios &)
{
  PAssertAlways("Cannot copy ios");
}


void PChannel::Construct()
{
  os_handle = -1;
  px_readThread = NULL;
  px_writeThread = NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
// PChannel::PXSetIOBlock
//   This function is used to perform IO blocks.
//   If the return value is FALSE, then the select call either
//   returned an error or a timeout occurred. The member variable lastError
//   can be used to determine which error occurred
//

BOOL PChannel::PXSetIOBlock(int type, const PTimeInterval & timeout)
{
  ErrorGroup group;
  switch (type) {
    case PXReadBlock :
      group = LastReadError;
      break;
    case PXWriteBlock :
      group = LastWriteError;
      break;
    default :
      group = LastGeneralError;
  }

  if (os_handle < 0)
    return SetErrorValues(NotOpen, EBADF, group);

  PThread * blockedThread = PThread::Current();

  px_threadMutex.Wait();
  if (type != PXWriteBlock) {
    PAssert(px_readThread == NULL, "Attempt to read from multiple threads.");
    px_readThread = blockedThread;
  }
  else {
    PTRACE(4, "PWLib\tBlocking on write.");
    px_writeMutex.Wait();
    px_writeThread = blockedThread;
  }
  px_threadMutex.Signal();

  int stat = blockedThread->PXBlockOnIO(os_handle, type, timeout);

  px_threadMutex.Wait();
  if (type != PXWriteBlock)
    px_readThread = NULL;
  else {
    px_writeThread = NULL;
    px_writeMutex.Signal();
  }
  px_threadMutex.Signal();

  // if select returned < 0, then covert errno into lastError and return FALSE
  if (stat < 0)
    return ConvertOSError(stat, group);

  // if the select succeeded, then return TRUE
  if (stat > 0) 
    return TRUE;

  // otherwise, a timeout occurred so return FALSE
  return SetErrorValues(Timeout, ETIMEDOUT, group);
}


BOOL PChannel::Read(void * buf, PINDEX len)
{
  lastReadCount = 0;

  if (os_handle < 0)
    return SetErrorValues(NotOpen, EBADF, LastReadError);

  if (!PXSetIOBlock(PXReadBlock, readTimeout)) 
    return FALSE;

  if (ConvertOSError(lastReadCount = ::read(os_handle, buf, len), LastReadError))
    return lastReadCount > 0;

  lastReadCount = 0;
  return FALSE;
}


BOOL PChannel::Write(const void * buf, PINDEX len)
{
  // if the os_handle isn't open, no can do
  if (os_handle < 0)
    return SetErrorValues(NotOpen, EBADF, LastWriteError);

  // flush the buffer before doing a write
  IOSTREAM_MUTEX_WAIT();
  flush();
  IOSTREAM_MUTEX_SIGNAL();

  lastWriteCount = 0;
  
  while (len > 0) {

    int result;
    while ((result = ::write(os_handle, ((char *)buf)+lastWriteCount, len)) < 0) {
      if (errno != EWOULDBLOCK)
        return ConvertOSError(-1, LastWriteError);

      if (!PXSetIOBlock(PXWriteBlock, writeTimeout))
        return FALSE;
    }

    lastWriteCount += result;
    len -= result;
  }

#if !defined(P_PTHREADS) && !defined(P_MAC_MPTHREADS)
  PThread::Yield(); // Starvation prevention
#endif

  // Reset all the errors.
  return ConvertOSError(0, LastWriteError);
}

BOOL PChannel::Close()
{
  if (os_handle < 0)
    return SetErrorValues(NotOpen, EBADF);
  
  return ConvertOSError(PXClose());
}


static void AbortIO(PThread * & thread, PMutex & mutex)
{
  mutex.Wait();
  while (thread != NULL) {
    thread->PXAbortIO();
    mutex.Signal();
    PThread::Yield();
    mutex.Wait();
  }
  mutex.Signal();
}

BOOL PChannel::PXClose()
{
  if (os_handle < 0)
    return -1;

  // make sure we don't have any problems
  IOSTREAM_MUTEX_WAIT();
  flush();
  int handle = os_handle;
  os_handle = -1;
  IOSTREAM_MUTEX_SIGNAL();

#if !defined(P_PTHREADS) && !defined(BE_THREADS) && !defined(P_MAC_MPTHREADS)
  // abort any I/O block using this os_handle
  PProcess::Current().PXAbortIOBlock(handle);

#ifndef BE_BONELESS
  DWORD cmd = 0;
  ::ioctl(handle, FIONBIO, &cmd);
#endif
#endif

  AbortIO(px_readThread, px_threadMutex);
  AbortIO(px_writeThread, px_threadMutex);

  int stat;
  do {
    stat = ::close(handle);
  } while (stat == -1 && errno == EINTR);

  return stat;
}

PString PChannel::GetErrorText(Errors, int osError = 0)
{
  if (osError == 0)
    return PString();

  const char * err = strerror(osError);
  if (err != NULL)
    return err;

  return psprintf("Unknown error %d", osError);
}


BOOL PChannel::ConvertOSError(int err, Errors & lastError, int & osError)

{
  osError = (err >= 0) ? 0 : errno;

  switch (osError) {
    case 0 :
      lastError = NoError;
      return TRUE;

    case EMSGSIZE:
      lastError = BufferTooSmall;
      break;

    case EBADF:  // will get EBADF if a read/write occurs after closing. This must return Interrupted
    case EINTR:
      lastError = Interrupted;
      break;

    case EEXIST:
      lastError = FileExists;
      break;

    case EISDIR:
    case EROFS:
    case EACCES:
    case EPERM:
      lastError = AccessDenied;
      break;

#ifndef __BEOS__
    case ETXTBSY:
      lastError = DeviceInUse;
      break;
#endif

    case EFAULT:
    case ELOOP:
    case EINVAL:
      lastError = BadParameter;
      break;

    case ENOENT :
    case ENAMETOOLONG:
    case ENOTDIR:
      lastError = NotFound;
      break;

    case EMFILE:
    case ENFILE:
    case ENOMEM :
      lastError = NoMemory;
      break;

    case ENOSPC:
      lastError = DiskFull;
      break;

    default :
      lastError = Miscellaneous;
      break;
  }
  return FALSE;
}


///////////////////////////////////////////////////////////////////////////////

