/*
 * $Id: osutils.cxx,v 1.55 1996/03/05 14:05:51 robertj Exp $
 *
 * Portable Windows Library
 *
 * Operating System Classes Inline Function Definitions
 *
 * Copyright 1993 Equivalence
 *
 * $Log: osutils.cxx,v $
 * Revision 1.55  1996/03/05 14:05:51  robertj
 * Fixed some more bugs in scheduling.
 *
 * Revision 1.54  1996/03/04 12:22:46  robertj
 * Fixed threading for unix stack check and loop list start point.
 *
 * Revision 1.53  1996/03/03 07:39:51  robertj
 * Fixed bug in thread scheduler for correct termination of "current" thread.
 *
 * Revision 1.52  1996/03/02 03:24:48  robertj
 * Changed timer thread to update timers periodically, this allows timers to be
 *    views dynamically by other threads.
 * Added automatic deletion of thread object instances on thread completion.
 *
 * Revision 1.51  1996/02/25 11:15:27  robertj
 * Added platform dependent Construct function to PProcess.
 *
 * Revision 1.50  1996/02/25 03:09:46  robertj
 * Added consts to all GetXxxx functions in PConfig.
 *
 * Revision 1.49  1996/02/15 14:44:09  robertj
 * Used string constructor for PTime, more "efficient".
 *
 * Revision 1.48  1996/02/13 12:59:30  robertj
 * Changed GetTimeZone() so can specify standard/daylight time.
 * Split PTime into separate module after major change to ReadFrom().
 *
 * Revision 1.47  1996/02/08 12:26:55  robertj
 * Changed time for full support of time zones.
 *
 * Revision 1.46  1996/02/03 11:06:49  robertj
 * Added string constructor for times, parses date/time from string.
 *
 * Revision 1.45  1996/01/28 14:09:39  robertj
 * Fixed bug in time reading function for dates before 1980.
 * Fixed bug in time reading, was out by one month.
 * Added time functions to PConfig.
 *
 * Revision 1.44  1996/01/28 02:52:04  robertj
 * Added assert into all Compare functions to assure comparison between compatible objects.
 *
 * Revision 1.43  1996/01/23 13:16:30  robertj
 * Mac Metrowerks compiler support.
 * Fixed timers so background thread not created if a windows app.
 *
 * Revision 1.42  1996/01/03 23:15:39  robertj
 * Fixed some PTime bugs.
 *
 * Revision 1.41  1996/01/03 11:09:35  robertj
 * Added Universal Time and Time Zones to PTime class.
 *
 * Revision 1.39  1995/12/23 03:40:40  robertj
 * Changed version number system
 *
 * Revision 1.38  1995/12/10 11:41:12  robertj
 * Added extra user information to processes and applications.
 * Implemented timer support in text only applications with platform threads.
 * Fixed bug in non-platform threads and semaphore timeouts.
 *
 * Revision 1.37  1995/11/21 11:50:57  robertj
 * Added timeout on semaphore wait.
 *
 * Revision 1.36  1995/11/09 12:22:58  robertj
 * Fixed bug in stream when reading an FF (get EOF).
 *
 * Revision 1.35  1995/07/31 12:09:25  robertj
 * Added semaphore class.
 * Removed PContainer from PChannel ancestor.
 *
 * Revision 1.34  1995/06/04 12:41:08  robertj
 * Fixed bug in accessing argument strings with no argument.
 *
 * Revision 1.33  1995/04/25 11:30:06  robertj
 * Fixed Borland compiler warnings.
 *
 * Revision 1.32  1995/04/22 00:51:00  robertj
 * Changed file path strings to use PFilePath object.
 * Changed semantics of Rename().
 *
 * Revision 1.31  1995/04/02 09:27:31  robertj
 * Added "balloon" help.
 *
 * Revision 1.30  1995/04/01 08:30:58  robertj
 * Fixed bug in timeout code of timers.
 *
 * Revision 1.29  1995/01/27 11:15:17  robertj
 * Removed enum to int warning from GCC.
 *
 * Revision 1.28  1995/01/18  09:02:43  robertj
 * Added notifier to timer.
 *
 * Revision 1.27  1995/01/15  04:57:15  robertj
 * Implemented PTime::ReadFrom.
 * Fixed flush of iostream at end of file.
 *
 * Revision 1.26  1995/01/11  09:45:14  robertj
 * Documentation and normalisation.
 *
 * Revision 1.25  1995/01/10  11:44:15  robertj
 * Removed PString parameter in stdarg function for GNU C++ compatibility.
 *
 * Revision 1.24  1995/01/09  12:31:51  robertj
 * Removed unnecesary return value from I/O functions.
 *
 * Revision 1.23  1994/12/12  10:09:24  robertj
 * Fixed flotain point configuration variable format.
 *
 * Revision 1.22  1994/11/28  12:38:23  robertj
 * Async write functions should have const pointer.
 *
 * Revision 1.21  1994/10/30  11:36:58  robertj
 * Fixed missing space in tine format string.
 *
 * Revision 1.20  1994/10/23  03:46:41  robertj
 * Shortened OS error assert.
 *
 * Revision 1.19  1994/09/25  10:51:04  robertj
 * Fixed error conversion code to use common function.
 * Added pipe channel.
 *
 * Revision 1.18  1994/08/21  23:43:02  robertj
 * Moved meta-string transmitter from PModem to PChannel.
 * Added SuspendBlock state to cooperative multi-threading to fix logic fault.
 * Added "force" option to Remove/Rename etc to override write protection.
 * Added common entry point to convert OS error to PChannel error.
 *
 * Revision 1.17  1994/08/04  12:57:10  robertj
 * Changed CheckBlock() to better name.
 * Moved timer porcessing so is done at every Yield().
 *
 * Revision 1.16  1994/08/01  03:39:42  robertj
 * Fixed temporary variable problem with GNU C++
 *
 * Revision 1.15  1994/07/27  05:58:07  robertj
 * Synchronisation.
 *
 * Revision 1.14  1994/07/25  03:39:22  robertj
 * Fixed problems with C++ temporary variables.
 *
 * Revision 1.13  1994/07/21  12:33:49  robertj
 * Moved cooperative threads to common.
 *
 * Revision 1.12  1994/07/17  10:46:06  robertj
 * Fixed timer bug.
 * Moved handle from file to channel.
 * Changed args to use container classes.
 *
 * Revision 1.11  1994/07/02  03:03:49  robertj
 * Time interval and timer redesign.
 *
 * Revision 1.10  1994/06/25  11:55:15  robertj
 * Unix version synchronisation.
 *
 * Revision 1.9  1994/04/20  12:17:44  robertj
 * assert changes
 *
 * Revision 1.8  1994/04/01  14:05:06  robertj
 * Text file streams
 *
 * Revision 1.7  1994/03/07  07:47:00  robertj
 * Major upgrade
 *
 * Revision 1.6  1994/01/03  04:42:23  robertj
 * Mass changes to common container classes and interactors etc etc etc.
 *
 * Revision 1.5  1993/12/31  06:53:02  robertj
 * Made inlines optional for debugging purposes.
 *
 * Revision 1.4  1993/12/29  04:41:26  robertj
 * Mac port.
 *
 * Revision 1.3  1993/11/20  17:26:28  robertj
 * Removed separate osutil.h
 *
 * Revision 1.2  1993/08/31  03:38:02  robertj
 * G++ needs explicit casts for char * / void * interchange.
 *
 * Revision 1.1  1993/08/27  18:17:47  robertj
 * Initial revision
 *
 */

#include <ptlib.h>

#include <ctype.h>


///////////////////////////////////////////////////////////////////////////////
// PTimer

#if defined(_PTIMER)

PTimer::PTimer(long millisecs, int seconds, int minutes, int hours, int days)
  : resetTime(millisecs, seconds, minutes, hours, days)
{
  state = Stopped;
  inTimeout = FALSE;
  StartRunning(TRUE);
}


PTimer::PTimer(const PTimeInterval & time)
  : resetTime(time)
{
  state = Stopped;
  inTimeout = FALSE;
  StartRunning(TRUE);
}


PTimer & PTimer::operator=(DWORD milliseconds)
{
  resetTime = milliseconds;
  StartRunning(oneshot);
  return *this;
}


PTimer & PTimer::operator=(const PTimeInterval & time)
{
  resetTime = time;
  StartRunning(oneshot);
  return *this;
}


PTimer::~PTimer()
{
  PAssert(!inTimeout, "Timer destroyed in OnTimeout()");
  if (state == Running) {
    PProcess::Current()->GetTimerList()->Remove(this);
#if defined(P_PLATFORM_HAS_THREADS)
    PProcess::Current()->SignalTimerChange();
#endif
  }
}


void PTimer::RunContinuous(const PTimeInterval & time)
{
  resetTime = time;
  StartRunning(FALSE);
}


void PTimer::StartRunning(BOOL once)
{
  if (state == Running && !inTimeout) {
    PProcess::Current()->GetTimerList()->Remove(this);
#if defined(P_PLATFORM_HAS_THREADS)
    PProcess::Current()->SignalTimerChange();
#endif
  }

  PTimeInterval::operator=(resetTime);
  oneshot = once;
  state = (*this) != 0 ? Running : Stopped;

  if (state == Running && !inTimeout) {
    PProcess::Current()->GetTimerList()->Append(this);
#if defined(P_PLATFORM_HAS_THREADS)
    PProcess::Current()->SignalTimerChange();
#endif
  }
}


void PTimer::Stop()
{
  if (state == Running && !inTimeout) {
    PProcess::Current()->GetTimerList()->Remove(this);
#if defined(P_PLATFORM_HAS_THREADS)
    PProcess::Current()->SignalTimerChange();
#endif
  }
  state = Stopped;
  SetInterval(0);
}


void PTimer::Pause()
{
  if (state == Running) {
    if (!inTimeout) {
      PProcess::Current()->GetTimerList()->Remove(this);
#if defined(P_PLATFORM_HAS_THREADS)
      PProcess::Current()->SignalTimerChange();
#endif
    }
    state = Paused;
  }
}


void PTimer::Resume()
{
  if (state == Paused) {
    if (!inTimeout) {
      PProcess::Current()->GetTimerList()->Append(this);
#if defined(P_PLATFORM_HAS_THREADS)
      PProcess::Current()->SignalTimerChange();
#endif
    }
    state = Running;
  }
}


void PTimer::OnTimeout()
{
  if (!callback.IsNULL())
    callback(*this, state == Running);
}


BOOL PTimer::Process(const PTimeInterval & delta, PTimeInterval & minTimeLeft)
{
  if (inTimeout)
    return FALSE;

  inTimeout = TRUE;

  operator-=(delta);

  if (*this > 0) {
    if (*this < minTimeLeft)
      minTimeLeft = *this;
    inTimeout = FALSE;
    return FALSE;
  }

  if (oneshot) {
    operator=(PTimeInterval(0));
    state = Stopped;
  }
  else {
    operator=(resetTime);
    if (resetTime < minTimeLeft)
      minTimeLeft = resetTime;
  }

  OnTimeout();

  inTimeout = FALSE;

  return state != Running;
}


PTimeInterval PTimerList::Process()
{
  PTimeInterval now = PTimer::Tick();
  PTimeInterval sampleTime = now - lastSample;
  if (now < lastSample)
    sampleTime += PMaxTimeInterval;
  lastSample = now;

  PTimeInterval minTimeLeft = PMaxTimeInterval;
  for (PINDEX i = 0; i < GetSize(); i++) {
    if (((PTimer *)GetAt(i))->Process(sampleTime, minTimeLeft))
      RemoveAt(i--);
  }

  return minTimeLeft;
}


#if defined(P_PLATFORM_HAS_THREADS)

PProcess::TimerThread::TimerThread()
  : PThread(1000, NoAutoDeleteThread, StartImmediate, LowPriority)
{
}


void PProcess::TimerThread::Main()
{
  for (;;) {
    if (PProcess::Current()->GetTimerList()->Process() == PMaxTimeInterval)
      semaphore.Wait();
    else
      semaphore.Wait(5000);
  }
}


void PProcess::SignalTimerChange()
{
  if (timerThread == (PThread *)-1)
    return;
  if (timerThread == NULL)
    timerThread = PNEW TimerThread;
  timerThread->semaphore.Signal();
}


#endif

#endif


///////////////////////////////////////////////////////////////////////////////
// PChannel

#if defined(_PCHANNEL)

PChannelStreamBuffer::PChannelStreamBuffer(PChannel * chan)
  : channel(PAssertNULL(chan))
{
}


int PChannelStreamBuffer::overflow(int c)
{
  if (pbase() == NULL) {
    if (eback() == 0)
      setp(buffer, &buffer[sizeof(buffer)]);
    else {
      char * halfway = &buffer[sizeof(buffer)/2];
      setp(buffer, halfway);
      setg(halfway, &buffer[sizeof(buffer)], &buffer[sizeof(buffer)]);
    }
  }

  int bufSize = pptr() - pbase();
  if (bufSize > 0) {
    setp(pbase(), epptr());
    if (!channel->Write(pbase(), bufSize))
      return EOF;
  }

  if (c != EOF) {
    char ch = (char)c;
    if (!channel->Write(&ch, 1))
      return EOF;
  }

  return 0;
}


int PChannelStreamBuffer::underflow()
{
  if (eback() == NULL) {
    if (pbase() == 0)
      setg(buffer, &buffer[sizeof(buffer)], &buffer[sizeof(buffer)]);
    else {
      char * halfway = &buffer[sizeof(buffer)/2];
      setp(buffer, halfway);
      setg(halfway, &buffer[sizeof(buffer)], &buffer[sizeof(buffer)]);
    }
  }

  if (gptr() != egptr())
    return (BYTE)*gptr();

  if (!channel->Read(eback(), egptr() - eback()) ||
                                  channel->GetErrorCode() != PChannel::NoError)
    return EOF;

  PINDEX count = channel->GetLastReadCount();
  char * p = egptr() - count;
  memmove(p, eback(), count);
  setg(eback(), p, egptr());
  return (BYTE)*p;
}


int PChannelStreamBuffer::sync()
{
  int inAvail = egptr() - gptr();
  if (inAvail > 0) {
    setg(eback(), egptr(), egptr());
    if (channel->IsDescendant(PFile::Class()))
      ((PFile *)channel)->SetPosition(-inAvail, PFile::Current);
  }

  if (pptr() > pbase())
    return overflow();

  return 0;
}


streampos PChannelStreamBuffer::seekoff(streamoff off,
#ifdef __MWERKS__
                                        ios::seekdir dir, ios::openmode)
#else
                                        ios::seek_dir dir, int)
#endif
{
  sync();
  if (!channel->IsDescendant(PFile::Class()))
    return -1;
  ((PFile *)channel)->SetPosition(off, (PFile::FilePositionOrigin)dir);
  return ((PFile *)channel)->GetPosition();
}


PChannel::PChannel()
  : readTimeout(PMaxTimeInterval), writeTimeout(PMaxTimeInterval)
{
  os_handle = -1;
  osError = 0;
  lastError = NoError;
  lastReadCount = lastWriteCount = 0;
  init(PNEW PChannelStreamBuffer(this));
  Construct();
}


PChannel::~PChannel()
{
  flush();
  delete (PChannelStreamBuffer *)rdbuf();
  init(NULL);
}


BOOL PChannel::IsOpen() const
{
  return os_handle >= 0;
}


int PChannel::ReadChar()
{
  BYTE c;
  BOOL retVal = Read(&c, 1);
  return (retVal && lastReadCount == 1) ? c : -1;
}


PString PChannel::ReadString(PINDEX len)
{
  PString str;
  if (!Read(str.GetPointer(len), len))
    return PString();
  str.SetSize(lastReadCount+1);
  return str;
}


BOOL PChannel::ReadAsync(void * buf, PINDEX len)
{
  BOOL retVal = Read(buf, len);
  OnReadComplete(buf, lastReadCount);
  return retVal;
}


void PChannel::OnReadComplete(void *, PINDEX)
{
}


BOOL PChannel::WriteChar(int c)
{
  PAssert(c >= 0 && c < 256, PInvalidParameter);
  char buf = (char)c;
  return Write(&buf, 1);
}


BOOL PChannel::WriteAsync(const void * buf, PINDEX len)
{
  BOOL retVal = Write(buf, len);
  OnWriteComplete(buf, lastWriteCount);
  return retVal;
}


void PChannel::OnWriteComplete(const void *, PINDEX)
{
}


enum {
  NextCharEndOfString = -1,
  NextCharDelay = -2,
  NextCharSend = -3,
  NextCharWait = -4
};


static int HexDigit(char c)
{
  if (!isxdigit(c))
    return 0;

  int hex = c - '0';
  if (hex < 10)
    return hex;

  hex -= 'A' - '9' - 1;
  if (hex < 16)
    return hex;

  return hex - ('a' - 'A');
}


static int GetNextChar(const PString & command,
                                    PINDEX & pos, PTimeInterval * time = NULL)
{
  int temp;

  if (command[pos] == '\0')
    return NextCharEndOfString;

  if (command[pos] != '\\')
    return command[pos++];

  switch (command[++pos]) {
    case '\0' :
      return NextCharEndOfString;

    case 'a' : // alert (ascii value 7)
      pos++;
      return 7;

    case 'b' : // backspace (ascii value 8)
      pos++;
      return 8;

    case 'f' : // formfeed (ascii value 12)
      pos++;
      return 12;

    case 'n' : // newline (ascii value 10)
      pos++;
      return 10;

    case 'r' : // return (ascii value 13)
      pos++;
      return 13;

    case 't' : // horizontal tab (ascii value 9)
      pos++;
      return 9;

    case 'v' : // vertical tab (ascii value 11)
      pos++;
      return 11;

    case 'x' : // followed by hh  where nn is hex number (ascii value 0xhh)
      if (isxdigit(command[++pos])) {
        temp = HexDigit(command[pos++]);
        if (isxdigit(command[pos]))
          temp += HexDigit(command[pos++]);
        return temp;
      }
      return command[pos];

    case 's' :
      pos++;
      return NextCharSend;

    case 'd' : // ns  delay for n seconds/milliseconds
    case 'w' :
      temp = command[pos] == 'd' ? NextCharDelay : NextCharWait;
      long milliseconds = 0;
      while (isdigit(command[++pos]))
        milliseconds = milliseconds*10 + command[pos] - '0';
      if (milliseconds <= 0)
        milliseconds = 1;
      if (command[pos] == 'm')
        pos++;
      else {
        milliseconds *= 1000;
        if (command[pos] == 's')
          pos++;
      }
      if (time != NULL)
        *time = milliseconds;
      return temp;
  }

  if (command[pos] < '0' || command[pos] > '7')
    return command[pos++];

  // octal number
  temp = command[pos++] - '0';
  if (command[pos] < '0' || command[pos] > '7')
    return temp;

  temp += command[pos++] - '0';
  if (command[pos] < '0' || command[pos] > '7')
    return temp;

  temp += command[pos++] - '0';
  return temp;
}


static BOOL ReceiveString(int nextChar,
                            const PString & reply, PINDEX & pos, PINDEX start)
{
  if (nextChar != GetNextChar(reply, pos)) {
    pos = start;
    return FALSE;
  }

  PINDEX dummyPos = pos;
  return GetNextChar(reply, dummyPos) < 0;
}


static int ReadCharWithTimeout(PChannel & channel, PTimeInterval & timeout)
{
  channel.SetReadTimeout(timeout);
  PTimeInterval startTick = PTimer::Tick();
  int c;
  if ((c = channel.ReadChar()) < 0) // Timeout or aborted
    return -1;
  timeout -= PTimer::Tick() - startTick;
  return c;
}


BOOL PChannel::SendCommandString(const PString & command)
{
  abortCommandString = FALSE;

  int nextChar;
  PINDEX sendPosition = 0;
  PTimeInterval timeout;
  SetWriteTimeout(10000);

  while (!abortCommandString) { // not aborted
    nextChar = GetNextChar(command, sendPosition, &timeout);
    switch (nextChar) {
      default :
        if (!WriteChar(nextChar))
          return FALSE;
        break;

      case NextCharEndOfString :
        return TRUE;  // Success!!

      case NextCharSend :
        break;

      case NextCharDelay : // Delay in send
        PThread::Current()->Sleep(timeout);
        break;

      case NextCharWait : // Wait for reply
        PINDEX receivePosition = sendPosition;
        if (GetNextChar(command, receivePosition) < 0) {
          SetReadTimeout(timeout);
          while (ReadChar() >= 0)
            if (abortCommandString) // aborted
              return FALSE;
        }
        else {
          receivePosition = sendPosition;
          do {
            if (abortCommandString) // aborted
              return FALSE;
            if ((nextChar = ReadCharWithTimeout(*this, timeout)) < 0)
              return FALSE;
          } while (!ReceiveString(nextChar,
                                     command, receivePosition, sendPosition));
//          nextChar = GetNextChar(command, receivePosition);
          sendPosition = receivePosition;
        }
    }
  }

  return FALSE;
}


#endif


///////////////////////////////////////////////////////////////////////////////
// PFile

#if defined(_PFILE)

PFile::~PFile()
{
  Close();
}


PObject::Comparison PFile::Compare(const PObject & obj) const
{
  PAssert(obj.IsDescendant(PFile::Class()), PInvalidCast);
  return path.Compare(((const PFile &)obj).path);
}


BOOL PFile::Rename(const PString & newname, BOOL force)
{
  Close();

  if (!ConvertOSError(Rename(path, newname, force) ? 0 : -1))
    return FALSE;

  path = path.GetDirectory() + newname;
  return TRUE;
}


BOOL PFile::Close()
{
  if (os_handle < 0) {
    osError = EBADF;
    lastError = NotOpen;
    return FALSE;
  }

  flush();

  BOOL ok = ConvertOSError(_close(os_handle));

  os_handle = -1;

  if (removeOnClose)
    Remove();

  return ok;
}


BOOL PFile::Read(void * buffer, PINDEX amount)
{
  flush();
  lastReadCount = _read(GetHandle(), buffer, amount);
  return ConvertOSError(lastReadCount) && lastReadCount > 0;
}


BOOL PFile::Write(const void * buffer, PINDEX amount)
{
  flush();
  lastWriteCount = _write(GetHandle(), buffer, amount);
  return ConvertOSError(lastWriteCount) && lastWriteCount >= amount;
}


BOOL PFile::Open(const PFilePath & name, OpenMode  mode, int opts)
{
  SetFilePath(name);
  return Open(mode, opts);
}


off_t PFile::GetLength() const
{
  off_t pos = _lseek(GetHandle(), 0, SEEK_CUR);
  off_t len = _lseek(GetHandle(), 0, SEEK_END);
  PAssertOS(_lseek(GetHandle(), pos, SEEK_SET) == pos);
  return len;
}


BOOL PFile::IsEndOfFile() const
{
  ((PFile *)this)->flush();
  return GetPosition() >= GetLength();
}


BOOL PFile::SetPosition(long pos, FilePositionOrigin origin)
{
  return _lseek(GetHandle(), pos, origin) == pos;
}


BOOL PFile::Copy(const PFilePath & oldname, const PFilePath & newname, BOOL force)
{
  PFile oldfile(oldname, ReadOnly);
  if (!oldfile.IsOpen())
    return FALSE;

  PFile newfile(newname,
                   WriteOnly, Create|Truncate|(force ? MustExist : Exclusive));
  if (!newfile.IsOpen())
    return FALSE;

  PCharArray buffer(10000);

  off_t amount = oldfile.GetLength();
  while (amount > 10000) {
    if (!oldfile.Read(buffer.GetPointer(), 10000))
      return FALSE;
    if (!newfile.Write((const char *)buffer, 10000))
      return FALSE;
    amount -= 10000;
  }

  if (!oldfile.Read(buffer.GetPointer(), (int)amount))
    return FALSE;
  if (!oldfile.Write((const char *)buffer, (int)amount))
    return FALSE;

  return newfile.Close();
}


#endif


///////////////////////////////////////////////////////////////////////////////
// PStructuredFile

#if defined(_PSTRUCTUREDFILE)

BOOL PStructuredFile::Read(void * buffer)
{
  return PFile::Read(buffer, structureSize);
}
      

BOOL PStructuredFile::Write(void * buffer)
{
  return PFile::Write(buffer, structureSize);
}


#endif


///////////////////////////////////////////////////////////////////////////////
// PPipeChannel

#if defined(_PPIPECHANNEL)

PBASEARRAY(PConstCharStarArray, const char *);

PPipeChannel::PPipeChannel(const PString & subProgram,
                 const PStringList & arguments, OpenMode mode, BOOL searchPath)
{
  PConstCharStarArray args(arguments.GetSize());
  for (PINDEX i = 0; i < arguments.GetSize(); i++)
    args[i] = arguments[i];
  Construct(subProgram, args, mode, searchPath);
}


PObject::Comparison PPipeChannel::Compare(const PObject & obj) const
{
  PAssert(obj.IsDescendant(PPipeChannel::Class()), PInvalidCast);
  return subProgName.Compare(((const PPipeChannel &)obj).subProgName);
}


PString PPipeChannel::GetName() const
{
  return subProgName;
}


#endif


///////////////////////////////////////////////////////////////////////////////
// PSerialChannel

#ifdef _PSERIALCHANNEL

PSerialChannel::PSerialChannel()
{
  Construct();
}


PSerialChannel::PSerialChannel(const PString & port, DWORD speed, BYTE data,
       Parity parity, BYTE stop, FlowControl inputFlow, FlowControl outputFlow)
{
  Construct();
  Open(port, speed, data, parity, stop, inputFlow, outputFlow);
}


PSerialChannel::PSerialChannel(PConfig & cfg)
{
  Construct();
  Open(cfg);
}


PSerialChannel::~PSerialChannel()
{
  Close();
}


static const char PortName[] = "PortName";
static const char PortSpeed[] = "PortSpeed";
static const char PortDataBits[] = "PortDataBits";
static const char PortParity[] = "PortParity";
static const char PortStopBits[] = "PortStopBits";
static const char PortInputFlow[] = "PortInputFlow";
static const char PortOutputFlow[] = "PortOutputFlow";


BOOL PSerialChannel::Open(PConfig & cfg)
{
  PStringList ports = GetPortNames();
  return Open(cfg.GetString(PortName, ports[0]),
              cfg.GetInteger(PortSpeed, 9600),
              (BYTE)cfg.GetInteger(PortDataBits, 8),
              (PSerialChannel::Parity)cfg.GetInteger(PortParity, 1),
              (BYTE)cfg.GetInteger(PortStopBits, 1),
              (PSerialChannel::FlowControl)cfg.GetInteger(PortInputFlow, 1),
              (PSerialChannel::FlowControl)cfg.GetInteger(PortOutputFlow, 1));
}


void PSerialChannel::SaveSettings(PConfig & cfg)
{
  cfg.SetString(PortName, GetName());
  cfg.SetInteger(PortSpeed, GetSpeed());
  cfg.SetInteger(PortDataBits, GetDataBits());
  cfg.SetInteger(PortParity, GetParity());
  cfg.SetInteger(PortStopBits, GetStopBits());
  cfg.SetInteger(PortInputFlow, GetInputFlowControl());
  cfg.SetInteger(PortOutputFlow, GetOutputFlowControl());
}


#endif


///////////////////////////////////////////////////////////////////////////////
// PModem

#ifdef _PMODEM

PModem::PModem()
{
  status = Unopened;
}


PModem::PModem(const PString & port, DWORD speed, BYTE data,
      Parity parity, BYTE stop, FlowControl inputFlow, FlowControl outputFlow)
  : PSerialChannel(port, speed, data, parity, stop, inputFlow, outputFlow)
{
  status = IsOpen() ? Uninitialised : Unopened;
}


PModem::PModem(PConfig & cfg)
{
  status = Open(cfg) ? Uninitialised : Unopened;
}


BOOL PModem::Close()
{
  status = Unopened;
  return PSerialChannel::Close();
}


BOOL PModem::Open(const PString & port, DWORD speed, BYTE data,
      Parity parity, BYTE stop, FlowControl inputFlow, FlowControl outputFlow)
{
  if (!PSerialChannel::Open(port,
                            speed, data, parity, stop, inputFlow, outputFlow))
    return FALSE;

  status = Uninitialised;
  return TRUE;
}


static const char ModemInit[] = "ModemInit";
static const char ModemDeinit[] = "ModemDeinit";
static const char ModemPreDial[] = "ModemPreDial";
static const char ModemPostDial[] = "ModemPostDial";
static const char ModemBusy[] = "ModemBusy";
static const char ModemNoCarrier[] = "ModemNoCarrier";
static const char ModemConnect[] = "ModemConnect";
static const char ModemHangUp[] = "ModemHangUp";

BOOL PModem::Open(PConfig & cfg)
{
  initCmd = cfg.GetString(ModemInit, "ATZ\\r\\w2sOK\\w100m");
  deinitCmd = cfg.GetString(ModemDeinit, "\\d2s+++\\d2sATH0\\r");
  preDialCmd = cfg.GetString(ModemPreDial, "ATDT");
  postDialCmd = cfg.GetString(ModemPostDial, "\\r");
  busyReply = cfg.GetString(ModemBusy, "BUSY");
  noCarrierReply = cfg.GetString(ModemNoCarrier, "NO CARRIER");
  connectReply = cfg.GetString(ModemConnect, "CONNECT");
  hangUpCmd = cfg.GetString(ModemHangUp, "\\d2s+++\\d2sATH0\\r");

  if (!PSerialChannel::Open(cfg))
    return FALSE;

  status = Uninitialised;
  return TRUE;
}


void PModem::SaveSettings(PConfig & cfg)
{
  PSerialChannel::SaveSettings(cfg);
  cfg.SetString(ModemInit, initCmd);
  cfg.SetString(ModemDeinit, deinitCmd);
  cfg.SetString(ModemPreDial, preDialCmd);
  cfg.SetString(ModemPostDial, postDialCmd);
  cfg.SetString(ModemBusy, busyReply);
  cfg.SetString(ModemNoCarrier, noCarrierReply);
  cfg.SetString(ModemConnect, connectReply);
  cfg.SetString(ModemHangUp, hangUpCmd);
}


BOOL PModem::CanInitialise() const
{
  switch (status) {
    case Unopened :
    case Initialising :
    case Dialling :
    case AwaitingResponse :
    case HangingUp :
    case Deinitialising :
    case SendingUserCommand :
      return FALSE;

    default :
      return TRUE;
  }
}


BOOL PModem::Initialise()
{
  if (CanInitialise()) {
    status = Initialising;
    if (SendCommandString(initCmd)) {
      status = Initialised;
      return TRUE;
    }
    status = InitialiseFailed;
  }
  return FALSE;
}


BOOL PModem::CanDeinitialise() const
{
  switch (status) {
    case Unopened :
    case Initialising :
    case Dialling :
    case AwaitingResponse :
    case Connected :
    case HangingUp :
    case Deinitialising :
    case SendingUserCommand :
      return FALSE;

    default :
      return TRUE;
  }
}


BOOL PModem::Deinitialise()
{
  if (CanDeinitialise()) {
    status = Deinitialising;
    if (SendCommandString(deinitCmd)) {
      status = Uninitialised;
      return TRUE;
    }
    status = DeinitialiseFailed;
  }
  return FALSE;
}


BOOL PModem::CanDial() const
{
  switch (status) {
    case Unopened :
    case Uninitialised :
    case Initialising :
    case InitialiseFailed :
    case Dialling :
    case AwaitingResponse :
    case Connected :
    case HangingUp :
    case Deinitialising :
    case DeinitialiseFailed :
    case SendingUserCommand :
      return FALSE;

    default :
      return TRUE;
  }
}


BOOL PModem::Dial(const PString & number)
{
  if (!CanDial())
    return FALSE;

  status = Dialling;
  if (!SendCommandString(preDialCmd + "\\s" + number + postDialCmd)) {
    status = DialFailed;
    return FALSE;
  }

  status = AwaitingResponse;

  PTimer timeout = 120000;
  PINDEX connectPosition = 0;
  PINDEX busyPosition = 0;
  PINDEX noCarrierPosition = 0;

  for (;;) {
    int nextChar;
    if ((nextChar = ReadCharWithTimeout(*this, timeout)) < 0)
      return FALSE;

    if (ReceiveString(nextChar, connectReply, connectPosition, 0))
      break;

    if (ReceiveString(nextChar, busyReply, busyPosition, 0)) {
      status = LineBusy;
      return FALSE;
    }

    if (ReceiveString(nextChar, noCarrierReply, noCarrierPosition, 0)) {
      status = NoCarrier;
      return FALSE;
    }
  }

  status = Connected;
  return TRUE;
}


BOOL PModem::CanHangUp() const
{
  switch (status) {
    case Unopened :
    case Uninitialised :
    case Initialising :
    case InitialiseFailed :
    case Dialling :
    case AwaitingResponse :
    case HangingUp :
    case Deinitialising :
    case SendingUserCommand :
      return FALSE;

    default :
      return TRUE;
  }
}


BOOL PModem::HangUp()
{
  if (CanHangUp()) {
    status = HangingUp;
    if (SendCommandString(hangUpCmd)) {
      status = Initialised;
      return TRUE;
    }
    status = HangUpFailed;
  }
  return FALSE;
}


BOOL PModem::CanSendUser() const
{
  switch (status) {
    case Unopened :
    case Uninitialised :
    case Initialising :
    case InitialiseFailed :
    case Dialling :
    case AwaitingResponse :
    case HangingUp :
    case Deinitialising :
    case SendingUserCommand :
      return FALSE;

    default :
      return TRUE;
  }
}


BOOL PModem::SendUser(const PString & str)
{
  if (CanSendUser()) {
    Status oldStatus = status;
    status = SendingUserCommand;
    if (SendCommandString(str)) {
      status = oldStatus;
      return TRUE;
    }
    status = oldStatus;
  }
  return FALSE;
}


void PModem::Abort()
{
  switch (status) {
    case Initialising :
      status = InitialiseFailed;
      break;
    case Dialling :
    case AwaitingResponse :
      status = DialFailed;
      break;
    case HangingUp :
      status = HangUpFailed;
      break;
    case Deinitialising :
      status = DeinitialiseFailed;
      break;
    default :
      break;
  }
}


BOOL PModem::CanRead() const
{
  switch (status) {
    case Unopened :
    case Initialising :
    case Dialling :
    case AwaitingResponse :
    case HangingUp :
    case Deinitialising :
    case SendingUserCommand :
      return FALSE;

    default :
      return TRUE;
  }
}


#endif


///////////////////////////////////////////////////////////////////////////////
// PConfig

#ifdef _PCONFIG

#ifndef _WIN32

BOOL PConfig::GetBoolean(const PString & section, const PString & key, BOOL dflt) const
{
  PString str = GetString(section, key, dflt ? "T" : "F").ToUpper();
  return str[0] == 'T' || str[0] == 'Y' || str.AsInteger() != 0;
}


void PConfig::SetBoolean(const PString & section, const PString & key, BOOL value)
{
  SetString(section, key, value ? "True" : "False");
}


long PConfig::GetInteger(const PString & section, const PString & key, long dflt) const
{
  PString str(PString::Signed, dflt);
  return GetString(section, key, str).AsInteger();
}


void PConfig::SetInteger(const PString & section, const PString & key, long value)
{
  PString str(PString::Signed, value);
  SetString(section, key, str);
}

#endif

double PConfig::GetReal(const PString & section, const PString & key, double dflt) const
{
  PString str(PString::Printf, "%g", dflt);
  return GetString(section, key, str).AsReal();
}


void PConfig::SetReal(const PString & section, const PString & key, double value)
{
  PString str(PString::Printf, "%g", value);
  SetString(section, key, str);
}


PTime PConfig::GetTime(const PString & section, const PString & key, const PTime & dflt) const
{
  return GetString(section, key, dflt.AsString());
}


void PConfig::SetTime(const PString & section, const PString & key, const PTime & value)
{
  SetString(section, key, value.AsString());
}


#endif

///////////////////////////////////////////////////////////////////////////////
// PArgList

#if defined(_PARGLIST)

PArgList::PArgList()
{
  arg_values = NULL;
  arg_count  = 0;
}


PArgList::PArgList(int theArgc, char ** theArgv, const char * theArgumentSpec)
{
  // get the program name and path
  SetArgs(theArgc, theArgv);

  if (theArgumentSpec != NULL)
    // we got an argument spec - so process them
    Parse(theArgumentSpec);
  else {
    // we have no argument spec, delay parsing the arguments until later
    arg_values = NULL;
    arg_count  = 0;
  }
}


PArgList::PArgList(int theArgc, char ** theArgv, const PString & theArgumentSpec)
{
  // get the program name and path
  SetArgs(theArgc, theArgv);
  // we got an argument spec - so process them
  Parse(theArgumentSpec);
}


void PArgList::SetArgs(int argc, char ** argv)
{
  // save argv and and argc for later
  arg_values = argv;
  arg_count = argc;
  shift = 0;
}


void PArgList::Parse(const char * theArgumentSpec)
{
  char  c;
  PINDEX p, l;

  // allocate and initialise storage
  argumentSpec = theArgumentSpec;
  l = argumentSpec.GetLength();
  optionCount.SetSize(l);
  argumentList.SetSize(l);

  arg_count -= shift;
  arg_values += shift;

  while (arg_count > 0 && arg_values[0][0] == '-') {
    if (arg_values[0][1] == '\0') {
      --arg_count;
      ++arg_values;
      return;
    }
    while ((c = *++arg_values[0]) != 0) {
      if ((p = argumentSpec.Find(c)) == P_MAX_INDEX)
        UnknownOption (c);
      else {
        optionCount[p]++;
        if (argumentSpec[p+1] == ':') {
          if (*++(arg_values[0]))
            argumentList[p] = arg_values[0];
          else {
            if (arg_count < 2) {
              optionCount[p] = 0;
              MissingArgument (c);
            }
            else {
              --arg_count;
              argumentList[p] = *++arg_values;
            }
          }
          break;
        }
      }
    }
    --arg_count;
    ++arg_values;
  }
}


PINDEX PArgList::GetOptionCount(char option) const
{
  PINDEX p = argumentSpec.Find(option);
  return (p == P_MAX_INDEX ? 0 : optionCount[p]);
}


PINDEX PArgList::GetOptionCount(const char * option) const
{
  // Future enhancement to have long option names
  return GetOptionCount(*option);
}


PString PArgList::GetOptionString(char option, const char * dflt) const
{
  PINDEX p = argumentSpec.Find(option);
  if (p != P_MAX_INDEX && argumentList.GetAt(p) != NULL)
    return argumentList[p];

  if (dflt != NULL)
    return dflt;

  return PString();
}


PString PArgList::GetOptionString(const char * option, const char * dflt) const
{
  // Future enhancement to have long option names
  return GetOptionString(*option, dflt);
}


PString PArgList::GetParameter(PINDEX num) const
{
  if ((num+shift) < arg_count)
    return arg_values[num+shift];

  IllegalArgumentIndex(num+shift);
  return PString();
}


void PArgList::Shift(int sh) 
{
  shift += sh;
  if (shift < 0)
    shift = 0;
  else if (shift >= arg_count)
    shift = arg_count-1;
}


void PArgList::IllegalArgumentIndex(PINDEX idx) const
{
  PError << "attempt to access undefined argument at index "
         << idx << endl;
}
 

void PArgList::UnknownOption(char option) const
{
  PError << "unknown option \"" << option << "\"\n";
}


void PArgList::MissingArgument(char option) const
{
  PError << "option \"" << option << "\" requires argument\n";
}


#endif


///////////////////////////////////////////////////////////////////////////////
// PThread

#ifndef P_PLATFORM_HAS_THREADS

void PThread::InitialiseProcessThread()
{
  basePriority = NormalPriority;  // User settable priority
  dynamicPriority = 0;            // Only thing running
  suspendCount = 0;               // Not suspended (would not be a good idea)
  ClearBlock();                   // No I/O blocking function
  status = Running;               // Thread is already running
  stackBase = NULL;
  link = this;
  ((PProcess*)this)->currentThread = this;
}


PThread::PThread(PINDEX stackSize,
                 AutoDeleteFlag deletion,
                 InitialSuspension start,
                 Priority priorityLevel)
{
  autoDelete = deletion == AutoDeleteThread;
  basePriority = priorityLevel;   // Threads user settable priority level
  dynamicPriority = 0;            // Run immediately

  suspendCount = start == StartSuspended ? 1 : 0;

  AllocateStack(stackSize);
  PAssert(stackBase != NULL, "Insufficient near heap for thread");

  status = Terminated; // Set to this so Restart() works
  Restart();
}


void PThread::Restart()
{
  if (status != Terminated) // Is already running
    return;

  ClearBlock();             // No I/O blocking function

  PThread * current = Current();
  link = current->link;
  current->link = this;

  status = Starting;
}


void PThread::Terminate()
{
  if (link == this || status == Terminated)
    return;   // Is only thread or already terminated

  BOOL doYield = status == Running;
  status = Terminating;
  if (doYield)
    Yield(); // Never returns from here
}


void PThread::Suspend(BOOL susp)
{
  // Suspend/Resume the thread
  if (susp)
    suspendCount++;
  else
    suspendCount--;

  switch (status) {
    case Running : // Suspending itself, yield to next thread
      if (IsSuspended()) {
        status = Suspended;
        Yield();
      }
      break;

    case Waiting :
      if (IsSuspended())
        status = Suspended;
      break;

    case BlockedIO :
      if (IsSuspended())
        status = SuspendedBlockIO;
      break;

    case BlockedSem :
      if (IsSuspended())
        status = SuspendedBlockSem;
      break;

    case Suspended :
      if (!IsSuspended())
        status = Waiting;
      break;

    case SuspendedBlockIO :
      if (!IsSuspended())
        status = BlockedIO;
      break;

    case SuspendedBlockSem :
      if (!IsSuspended())
        status = BlockedSem;
      break;

    default :
      break;
  }
}


void PThread::Sleep(const PTimeInterval & time)
{
  sleepTimer = time;
  switch (status) {
    case Running : // Suspending itself, yield to next thread
      status = Sleeping;
      Yield();
      break;

    case Waiting :
    case Suspended :
      status = Sleeping;
      break;

    default :
      break;
  }
}


void PThread::BeginThread()
{
  if (IsSuspended()) { // Begins suspended
    status = Suspended;
    Yield();
  }
  else
    status = Running;

  Main();

  status = Terminating;
  Yield(); // Never returns from here
}


void PThread::Yield()
{
  // Determine the next thread to schedule
  PProcess * process = PProcess::Current();

  // The following is static as the SwitchContext function invalidates all
  // automatic variables in this function call.
  static PThread * current;
  current = process->currentThread;

  if (current == process)
    process->GetTimerList()->Process();
  else
    PAssert((char *)&process > process->stackBase, "Stack overflow!");

  if (current->status == Running) {
    if (current->basePriority != HighestPriority && current->link != current)
      current->status = Waiting;
    else
      return;
  }

  static const int dynamicLevel[NumPriorities] = { -1, 3, 1, 0, 0 };
  current->dynamicPriority = dynamicLevel[current->basePriority];

  PThread * next = NULL; // Next thread to be scheduled
  PThread * prev = current; // Need the thread in the previous link
  PThread * start = current;  // Need thread in list that is the "start"
  PThread * thread = current->link;
  BOOL pass = 0;
  BOOL canUseLowest = TRUE;
  for (;;) {
    switch (thread->status) {
      case Waiting :
        if (thread->dynamicPriority == 0)
          next = thread;
        else if (thread->dynamicPriority > 0) {
          thread->dynamicPriority--;
          canUseLowest = FALSE;
        }
        else if (pass > 1 && canUseLowest)
          thread->dynamicPriority++;
        break;

      case Sleeping :
        if (!thread->sleepTimer.IsRunning()) {
          if (thread->IsSuspended())
            thread->status = Suspended;
          else
            next = thread;
        }
        break;

      case BlockedIO :
        if (thread->IsNoLongerBlocked()) {
          thread->ClearBlock();
          next = thread;
        }
        break;

      case BlockedSem :
      case SuspendedBlockSem :
        if (thread->blockingSemaphore->timeout == 0)
          thread->blockingSemaphore->Signal();
        break;

      case Starting :
        if (!thread->IsSuspended())
          next = thread;
        break;

      case Terminating :
        prev->link = thread->link;   // Unlink it from the list
        thread->status = Terminated;
        if (thread == start)         // If unlinking the "start" thread
          start = prev;      //    then we better make it still in list
        if (thread != current && thread->autoDelete)
          delete thread;
        break;

      default :
        break;
    }

    if (next != NULL) // Have a thread to run
      break;

    // Need to have previous thread so can unlink a terminating thread
    prev = thread;
    thread = thread->link;
    if (thread == start) {
      pass++;
      if (pass > 3) // Everything is blocked
        process->OperatingSystemYield();
    }
  }

  process->currentThread = next;
  if (next->status != Starting)
    next->status = Running;

  next->SwitchContext(current);

  if (current->status == Terminated && current->autoDelete)
    delete current;
}


#endif


///////////////////////////////////////////////////////////////////////////////
// PProcess

#if defined(_PPROCESS)

PProcess::PProcess(const char * manuf, const char * name,
                           WORD major, WORD minor, CodeStatus stat, WORD build)
  : manufacturer(manuf), productName(name)
{
  PProcessInstance = this;
  majorVersion = major;
  minorVersion = minor;
  status = stat;
  buildNumber = build;
  Construct();
}


PProcess::~PProcess()
{
#if defined(P_PLATFORM_HAS_THREADS)
  if (timerThread != (PThread *)-1)
    delete timerThread;
#endif
}


PObject::Comparison PProcess::Compare(const PObject & obj) const
{
  PAssert(obj.IsDescendant(PProcess::Class()), PInvalidCast);
  return productName.Compare(((const PProcess &)obj).productName);
}


void PProcess::PreInitialise(int argc, char ** argv)
{
  terminationValue = 0;

  arguments.SetArgs(argc-1, argv+1);

  executableFile = PString(argv[0]);

  if (productName.IsEmpty())
    productName = executableFile.GetTitle().ToLower();

  InitialiseProcessThread();
}


void PProcess::Terminate()
{
#ifdef _WINDLL
  FatalExit(terminationValue);
#else
  exit(terminationValue);
#endif
}


PString PProcess::GetVersion(BOOL full) const
{
  const char * const statusLetter[NumCodeStatuses] =
    { "alpha", "beta", "pl" };
  return psprintf(full ? "%u.%u%s%u" : "%u.%u",
                majorVersion, minorVersion, statusLetter[status], buildNumber);
}


///////////////////////////////////////////////////////////////////////////////
// PSemaphore

#ifndef P_PLATFORM_HAS_THREADS

PSemaphore::PSemaphore(unsigned initial, unsigned maxCount)
{
  PAssert(maxCount > 0, "Invalid semaphore maximum.");
  if (initial > maxCount)
    initial = maxCount;
  currentCount = initial;
  maximumCount = maxCount;
}


PSemaphore::~PSemaphore()
{
  PAssert(blockedThreads.IsEmpty(),
                        "Semaphore destroyed while still has blocked threads");
}


void PSemaphore::Wait()
{
  Wait(PMaxTimeInterval);
}


BOOL PSemaphore::Wait(const PTimeInterval & time)
{
  if (currentCount > 0)
    currentCount--;
  else {
    PThread * thread = PThread::Current();
    blockedThreads.Enqueue(thread);
    thread->blockingSemaphore = this;
    thread->status = PThread::BlockedSem;
    timeout = time;
    if (time == PMaxTimeInterval)
      timeout.Pause();
    PThread::Yield();
    if (timeout == 0)
      return FALSE;
  }
  return TRUE;
}


void PSemaphore::Signal()
{
  if (blockedThreads.GetSize() > 0) {
    PThread * thread = blockedThreads.Dequeue();
    switch (thread->status) {
      case PThread::BlockedSem :
        thread->status = PThread::Waiting;
        break;
      case PThread::SuspendedBlockSem :
        thread->status = PThread::Suspended;
        break;
      default:
        PAssertAlways("Semaphore unblock of thread that is not blocked");
    }
    thread->sleepTimer = 0;
    PThread::Yield();
  }
  else if (currentCount < maximumCount)
    currentCount++;
}


BOOL PSemaphore::WillBlock() const
{
  return currentCount == 0;
}


#endif


#endif


// End Of File ///////////////////////////////////////////////////////////////
