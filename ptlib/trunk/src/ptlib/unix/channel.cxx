/*
 * $Id: channel.cxx,v 1.8 1996/05/02 12:01:23 craigs Exp $
 *
 * Portable Windows Library
 *
 * Operating System Classes Implementation
 *
 * Copyright 1993 by Robert Jongbloed and Craig Southeren
 *
 * $Log: channel.cxx,v $
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

#include <ptlib.h>
#include <sys/ioctl.h>

#ifdef P_SUN4
#include <errno.h>
extern "C" int ioctl(int, int, void *);
#endif

///////////////////////////////////////////////////////////////////////////////
//
// PChannel::PXSetIOBlock
//   These functions are used to perform IO blocks.
//   If the return value is FALSE, then the select call either
//   returned an error or a timeout occurred. The member variable lastError
//   can be used to determine which error occurred
//

BOOL PChannel::PXSetIOBlock (int type, PTimeInterval timeout)
{
  return PXSetIOBlock(type, os_handle, timeout);
}

BOOL PChannel::PXSetIOBlock (int type, int blockHandle, PTimeInterval timeout)
{
  int stat = PThread::Current()->PXBlockOnIO(blockHandle, type, timeout);

  // if select returned < 0, then covert errno into lastError and return FALSE
  if (stat < 0)
    return ConvertOSError(-1);

  // if the select succeeded, then return TRUE
  if (stat > 0) 
    return TRUE;

  // otherwise, a timeout occurred so return FALSE
  lastError = Timeout;
  return FALSE;
}


BOOL PChannel::Read(void * buf, PINDEX len)
{
  if (os_handle < 0) {
    lastError = NotOpen;
    return FALSE;
  }

  if (!PXSetIOBlock(PXReadBlock, readTimeout)) 
    return FALSE;

  if (ConvertOSError(lastReadCount = ::read(os_handle, buf, len)))
    return lastReadCount > 0;

  lastReadCount = 0;
  return FALSE;
}


BOOL PChannel::Write(const void * buf, PINDEX len)
{

  // if the os_handle isn't open, no can do
  if (os_handle < 0) {
    lastError = NotOpen;
    return FALSE;
  }

  // flush the buffer before doing a write
  flush();

  lastWriteCount = 0;
  
  while (len > 0) {

    if (!PXSetIOBlock(PXWriteBlock, writeTimeout))
      return FALSE;

    int sendResult = ::write(os_handle,
                  ((const char *)buf)+lastWriteCount, len - lastWriteCount);

    if (!ConvertOSError(sendResult))
      return FALSE;

    lastWriteCount += sendResult;
    len -= sendResult;
  }

  return TRUE;
}

BOOL PChannel::Close()
{
  if (os_handle < 0) {
    lastError = NotOpen;
    return FALSE;
  }

  // flush the buffer before doing a close
  flush();

  // abort any I/O block using this os_handle
  PProcess::Current()->PXAbortIOBlock(os_handle);

  int handle = os_handle;
  os_handle = -1;
  DWORD cmd = 0;
  ::ioctl(handle, FIONBIO, &cmd);
  return ConvertOSError(::close(handle));
}

PString PChannel::GetErrorText() const
{
  return strerror(osError);
#if 0
#ifdef P_HPUX9
  if (osError > 0 && osError < sys_nerr)
    return sys_errlist[osError];
#else
  if (osError > 0 && osError < _sys_nerr)
    return _sys_errlist[osError];
#endif
  else
    return PString();
#endif
}

BOOL PChannel::ConvertOSError(int err)

{
  osError = (err >= 0) ? 0 : errno;

  switch (osError) {
    case 0 :
      lastError = NoError;
      return TRUE;

    case EINTR:
      lastError = Interrupted;
      break;

    case EEXIST:
      lastError = FileExists;
      break;

    case EISDIR:
    case EROFS:
    case EACCES:
      lastError = AccessDenied;
      break;

    case ETXTBSY:
      lastError = DeviceInUse;
      break;

    case EFAULT:
    case ELOOP:
    case EBADF:
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
