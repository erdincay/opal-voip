/*
 * tlibthrd.cxx
 *
 * Routines for pre-emptive threading system
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
 * $Log: tlibthrd.cxx,v $
 * Revision 1.121  2003/05/16 17:40:55  shawn
 * On Mac OS X, thread with the highest priority should use fixed priority
 * scheduling policy.  This avoids starvation caused by desktop activity.
 *
 * Revision 1.120  2003/05/02 00:58:40  dereks
 * Add test for linux at the end of PMutex::Signal. Thanks Robert!
 *
 * Revision 1.119  2003/05/02 00:39:11  dereks
 * Changes to make threading work on Redhat 9
 *
 * Revision 1.118  2003/04/24 12:03:13  rogerh
 * Calling pthread_mutex_unlock() on a mutex which is not locked can be
 * considered an error. NetBSD now enforce this error so we need to quickly
 * try locking the mutex before unlocking it in ~PThread and ~PSemaphore.
 *
 * Revision 1.117  2003/04/08 03:29:31  robertj
 * Fixed IsSuspeneded() so returns TRUE if thread not started yet, this makes
 *   it the same as the Win32 semantics.
 *
 * Revision 1.116  2003/03/10 15:37:00  rogerh
 * fix IsTerminated() function.
 *
 * Revision 1.115  2003/03/07 00:07:15  robertj
 * Fixed Mac OS X patch which broke every other platform.
 *
 * Revision 1.114  2003/03/06 08:58:48  rogerh
 * P_MACOSX now carries the OSRELEASE. Use this to enable better threads
 * support on Darwin 6.4. Submitted by Shawn.
 *
 * Revision 1.113  2003/02/20 23:32:00  robertj
 * More RTEMS support patches, thanks Sebastian Meyer.
 *
 * Revision 1.112  2003/01/24 10:21:06  robertj
 * Fixed issues in RTEMS support, thanks Vladimir Nesic
 *
 * Revision 1.111  2003/01/20 10:13:18  rogerh
 * NetBSD thread changes
 *
 * Revision 1.110  2003/01/20 10:05:46  rogerh
 * NetBSD thread changes
 *
 * Revision 1.109  2003/01/08 08:47:51  rogerh
 * Add new Sleep() function for GNU PTH threads.
 * Taken from NetBSD's package which uses PTH.
 * Note: I am not sure this works correctly.
 *
 * Revision 1.108  2003/01/06 18:49:15  rogerh
 * Back out pthead_kill to pthread_cancel change on NetBSD
 *
 * Revision 1.107  2002/12/11 05:39:26  robertj
 * Added logging for file handle changes.
 * Fixd bug where internal maxHandles not set when increased.
 *
 * Revision 1.106  2002/12/02 03:57:18  robertj
 * More RTEMS support patches, thank you Vladimir Nesic.
 *
 * Revision 1.105  2002/11/22 10:14:07  robertj
 * QNX port, thanks Xiaodan Tang
 *
 * Revision 1.104  2002/11/04 16:01:27  rogerh
 * Using pthread_cancel and not pthread_kill with SIGKILL to terminate a thread
 * On FreeBSD the thread does not have a handler for SIGKILL, it passes it up
 * to the main process which gets killed! Assume the other BSDs are the same.
 *
 * Revision 1.103  2002/10/24 00:40:56  robertj
 * Put back ability to terminate a thread from that threads context (removed
 *   in revision 1.101) but requires that destructor not do so.
 * Changed pipe close to allow for possible EINTR, and retry close.
 *
 * Revision 1.102  2002/10/24 00:25:13  robertj
 * Changed high load thread problem fix from the termination function to start
 *   function to finally, once and for all (I hope!) fix the race condition.
 *
 * Revision 1.101  2002/10/23 14:56:22  craigs
 * Fixed problem with pipe leak under some circumstances
 *
 * Revision 1.100  2002/10/23 04:29:32  robertj
 * Improved debugging for thread create/start/stop/destroy.
 * Fixed race condition bug if auto-delete thread starts and completes before
 *   pthread_create returns, PX_threadId is not set yet!
 *
 * Revision 1.99  2002/10/22 07:42:52  robertj
 * Added extra debugging for file handle and thread leak detection.
 *
 * Revision 1.98  2002/10/18 03:05:39  robertj
 * Fixed thread leak caused by fixing the thread crash a few revisions back,
 *   caused by strange pthreads behaviour, at least under Linux.
 *
 * Revision 1.97  2002/10/17 13:44:27  robertj
 * Port to RTEMS, thanks Vladimir Nesic.
 *
 * Revision 1.96  2002/10/17 12:57:24  robertj
 * Added ability to increase maximum file handles on a process.
 *
 * Revision 1.95  2002/10/16 11:26:29  rogerh
 * Add missing include. Noticed by Matthias on the GnomeMeeting IRC
 *
 * Revision 1.94  2002/10/10 03:09:48  robertj
 * Fixed high load race condition when starting threads.
 *
 * Revision 1.93  2002/10/05 05:22:43  robertj
 * Fixed adding GetThreadId() function.
 *
 * Revision 1.92  2002/10/01 06:27:48  robertj
 * Added bullet proofing against possible EINTR error returns on all pthread
 *   functions when under heavy load. Linux really should NOT do this, but ...
 *
 * Revision 1.91  2002/09/04 03:14:18  robertj
 * Backed out changes submitted by Martin Froehlich as they do not appear to
 *   actually do anything other than add a sychronisation point. The variables
 *   the patches intended to protect were already protected.
 * Fixed bug where if a PMutex was signalled by a thread that did not have it
 *   locked, it would assert but continue to alter PMutex variables such that
 *   a deadlock or crash is likely.
 *
 * Revision 1.90  2002/08/29 01:50:40  robertj
 * Changed the pthread_create so does retries if get EINTR or EAGAIN errors
 *   which indicate a (possibly) temporary resource limit.
 * Enabled and adjusted tracing.
 *
 * Revision 1.89  2002/08/22 13:05:57  craigs
 * Fixed problems with mutex implementation thanks to Martin Froehlich
 *
 * Revision 1.88  2002/07/15 06:56:59  craigs
 * Fixed missing brace
 *
 * Revision 1.87  2002/07/15 06:39:23  craigs
 * Added function to allow raising of per-process file handle limit
 *
 * Revision 1.86  2002/06/27 06:38:58  robertj
 * Changes to remove memory leak display for things that aren't memory leaks.
 *
 * Revision 1.85  2002/06/27 02:04:01  robertj
 * Fixed NetBSD compatibility issue, thanks Motoyuki OHMORI.
 *
 * Revision 1.84  2002/06/04 00:25:31  robertj
 * Fixed incorrectly initialised trace indent, thanks Artis Kugevics
 *
 * Revision 1.83  2002/05/21 09:13:00  robertj
 * Fixed problem when using posix recursive mutexes, thanks Artis Kugevics
 *
 * Revision 1.82  2002/04/24 01:11:37  robertj
 * Fixed problem with PTRACE_BLOCK indent level being correct across threads.
 *
 * Revision 1.81  2002/04/16 10:57:26  rogerh
 * Change WaitForTermination() so it does not use 100% CPU.
 * Reported by Andrea <ghittino@tiscali.it>
 *
 * Revision 1.80  2002/01/23 04:26:36  craigs
 * Added copy constructors for PSemaphore, PMutex and PSyncPoint to allow
 * use of default copy constructors for objects containing instances of
 * these classes
 *
 * Revision 1.79  2002/01/10 06:36:58  robertj
 * Fixed possible resource leak under Solaris, thanks Joegen Baclor
 *
 * Revision 1.78  2001/12/17 11:06:46  robertj
 * Removed assert on NULL PThread::Current(), can occur if thread from other
 *   subsystem to pwlib
 *
 * Revision 1.77  2001/10/03 05:11:50  robertj
 * Fixed PSyncPoint wait with timeout when have pending signals.
 *
 * Revision 1.76  2001/09/27 23:50:03  craigs
 * Fixed typo in PSemaphone destructor
 *
 * Revision 1.75  2001/09/24 10:09:48  rogerh
 * Fix an uninitialised variable problem.
 *
 * Revision 1.74  2001/09/20 05:38:25  robertj
 * Changed PSyncPoint to use pthread cond so timed wait blocks properly.
 * Also prevented semaphore from being created if subclass does not use it.
 *
 * Revision 1.73  2001/09/19 17:37:47  craigs
 * Added support for nested mutexes under Linux
 *
 * Revision 1.72  2001/09/18 06:53:35  robertj
 * Made sure suspend can't exit early if get spurious signal
 *
 * Revision 1.71  2001/09/18 05:56:03  robertj
 * Fixed numerous problems with thread suspend/resume and signals handling.
 *
 * Revision 1.70  2001/09/10 03:03:02  robertj
 * Major change to fix problem with error codes being corrupted in a
 *   PChannel when have simultaneous reads and writes in threads.
 * Changed threading so does not actually start thread until Resume(), makes
 *   the logic of start up much simpler and more portable.
 * Quite a bit of tidyin up of the pthreads code.
 *
 * Revision 1.69  2001/08/30 08:57:40  robertj
 * Changed calls to usleep to be PThread::Yield(), normalising single
 *   timeslice process swap out.
 *
 * Revision 1.68  2001/08/20 06:55:45  robertj
 * Fixed ability to have PMutex::Wait() with times less than one second.
 * Fixed small error in return value of block on I/O function, not critical.
 *
 * Revision 1.67  2001/08/07 02:50:03  craigs
 * Fixed potential race condition in IO blocking
 *
 * Revision 1.66  2001/07/09 13:23:37  rogerh
 * Fix a subtle bug in semaphore wait which showed up on FreeBSD
 *
 * Revision 1.65  2001/05/29 00:49:18  robertj
 * Added ability to put in a printf %x in thread name to get thread object
 *   address into user settable thread name.
 *
 * Revision 1.64  2001/05/23 00:18:55  robertj
 * Added support for real time threads, thanks Erland Lewin.
 *
 * Revision 1.63  2001/04/20 09:27:25  robertj
 * Fixed previous change for auto delete threads, must have thread ID zeroed.
 *
 * Revision 1.62  2001/04/20 09:09:05  craigs
 * Removed possible race condition whilst shutting down threads
 *
 * Revision 1.61  2001/03/20 06:44:25  robertj
 * Lots of changes to fix the problems with terminating threads that are I/O
 *   blocked, especially when doing orderly shutdown of service via SIGTERM.
 *
 * Revision 1.60  2001/03/14 01:16:11  robertj
 * Fixed signals processing, now uses housekeeping thread to handle signals
 *   synchronously. This also fixes issues with stopping PServiceProcess.
 *
 * Revision 1.59  2001/02/25 19:39:42  rogerh
 * Use a Semaphore on Mac OS X to support threads which are started as 'suspended'
 *
 * Revision 1.58  2001/02/24 14:49:22  rogerh
 * Add missing bracket
 *
 * Revision 1.57  2001/02/24 13:29:34  rogerh
 * Mac OS X change to avoid Assertion
 *
 * Revision 1.56  2001/02/24 13:24:24  rogerh
 * Add PThread support for Mac OS X and Darwin. There is one major issue. This
 * OS does not suport pthread_kill() and sigwait() so we cannot support the
 * Suspend() and Resume() functions to start and stop threads and we cannot
 * create new threads in 'suspended' mode.
 * Calling Suspend() raises an assertion. Calling Resume() does nothing.
 * Threads started in 'suspended' mode start immediatly.
 *
 * Revision 1.55  2001/02/21 22:48:42  robertj
 * Fixed incorrect test in PSemaphore::WillBlock() just added, thank Artis Kugevics.
 *
 * Revision 1.54  2001/02/20 00:21:14  robertj
 * Fixed major bug in PSemapahore::WillBlock(), thanks Tomas Heran.
 *
 * Revision 1.53  2000/12/21 12:36:32  craigs
 * Removed potential to stop threads twice
 *
 * Revision 1.52  2000/12/05 08:24:50  craigs
 * Fixed problem with EINTR causing havoc
 *
 * Revision 1.51  2000/11/16 11:06:38  rogerh
 * Add a better fix for the "user signal 2" aborts seen on FreeBSD 4.2 and above.
 * We need to sched_yeild() after the pthread_create() to make sure the new thread
 * actually has a chance to execute. The abort problem was caused when the
 * resume signal was issued before the thread was ready for it.
 *
 * Revision 1.50  2000/11/12 23:30:02  craigs
 * Added extra WaitForTermination to assist bug location
 *
 * Revision 1.49  2000/11/12 08:16:07  rogerh
 * This change and the previous change, make pthreads work on FreeBSD 4.2.
 * FreeBSD has improved its thread signal handling and now correctly generates a
 * SIGUSR2 signal on a thread (the Resume Signal).  However there was no handler
 * for this signal and applications would abort with "User signal 2".
 * So, a dummy sigResumeHandler has been added.
 *
 * Revision 1.48  2000/11/12 07:57:45  rogerh
 * *** empty log message ***
 *
 * Revision 1.47  2000/10/31 08:09:51  rogerh
 * Change return type of PX_GetThreadId() to save unnecessary typecasting
 *
 * Revision 1.46  2000/10/31 07:52:06  rogerh
 * Add type casts to allow the code to compile on FreeBSD 4.1.1
 *
 * Revision 1.45  2000/10/30 05:48:33  robertj
 * Added assert when get nested mutex.
 *
 * Revision 1.44  2000/10/24 03:32:40  robertj
 * Fixed problem where thread that uses PThread::Current() in dtor crashes.
 *
 * Revision 1.43  2000/10/20 06:11:48  robertj
 * Added function to change auto delete flag on a thread.
 *
 * Revision 1.42  2000/09/20 04:24:09  craigs
 * Added extra tracing, and removed segv on exit when using tracing
 *
 * Revision 1.41  2000/06/21 01:01:22  robertj
 * AIX port, thanks Wolfgang Platzer (wolfgang.platzer@infonova.at).
 *
 * Revision 1.40  2000/04/13 07:21:10  rogerh
 * Fix typo in #defined
 *
 * Revision 1.39  2000/04/11 11:38:49  rogerh
 * More NetBSD Pthread changes
 *
 * Revision 1.38  2000/04/10 11:47:02  rogerh
 * Add initial NetBSD pthreads support
 *
 * Revision 1.37  2000/04/06 12:19:49  rogerh
 * Add Mac OS X support submitted by Kevin Packard
 *
 * Revision 1.36  2000/03/20 22:56:34  craigs
 * Fixed problems with race conditions caused by testing or changing
 * attributes on a terminated thread. Only occured on a fast machine!
 *
 * Revision 1.35  2000/03/17 03:45:40  craigs
 * Fixed problem with connect call hanging
 *
 * Revision 1.34  2000/03/08 12:17:09  rogerh
 * Add OpenBSD support
 *
 * Revision 1.33  2000/02/29 13:18:21  robertj
 * Added named threads to tracing, thanks to Dave Harvey
 *
 * Revision 1.32  2000/01/20 08:20:57  robertj
 * FreeBSD v3 compatibility changes, thanks Roger Hardiman & Motonori Shindo
 *
 * Revision 1.31  1999/11/18 14:02:57  craigs
 * Fixed problem with houskeeping thread termination
 *
 * Revision 1.30  1999/11/15 01:12:56  craigs
 * Fixed problem with PSemaphore::Wait consuming 100% CPU
 *
 * Revision 1.29  1999/10/30 13:44:11  craigs
 * Added correct method of aborting socket operations asynchronously
 *
 * Revision 1.28  1999/10/24 13:03:30  craigs
 * Changed to capture io break signal
 *
 * Revision 1.27  1999/09/23 06:52:16  robertj
 * Changed PSemaphore to use Posix semaphores.
 *
 * Revision 1.26  1999/09/03 02:26:25  robertj
 * Changes to aid in breaking I/O locks on thread termination. Still needs more work esp in BSD!
 *
 * Revision 1.25  1999/09/02 11:56:35  robertj
 * Fixed problem with destroying PMutex that is already locked.
 *
 * Revision 1.24  1999/08/24 13:40:56  craigs
 * Fixed problem with condwait destorys failing on linux
 *
 * Revision 1.23  1999/08/23 05:33:45  robertj
 * Made last threading changes Linux only.
 *
 * Revision 1.22  1999/08/23 05:14:13  robertj
 * Removed blocking of interrupt signals as does not work in Linux threads.
 *
 * Revision 1.21  1999/07/30 00:40:32  robertj
 * Fixed problem with signal variable in non-Linux platforms
 *
 * Revision 1.20  1999/07/19 01:32:24  craigs
 * Changed signals used in pthreads code, is used by linux version.
 *
 * Revision 1.19  1999/07/15 13:10:55  craigs
 * Fixed problem with EINTR in nontimed sempahore waits
 *
 * Revision 1.18  1999/07/15 13:05:33  robertj
 * Fixed problem with getting EINTR in semaphore wait, is normal, not error.
 *
 * Revision 1.17  1999/07/11 13:42:13  craigs
 * pthreads support for Linux
 *
 * Revision 1.16  1999/05/12 03:29:20  robertj
 * Fixed problem with semaphore free, done at wrong time.
 *
 * Revision 1.15  1999/04/29 08:41:26  robertj
 * Fixed problems with uninitialised mutexes in PProcess.
 *
 * Revision 1.14  1999/03/16 10:54:16  robertj
 * Added parameterless version of WaitForTermination.
 *
 * Revision 1.13  1999/03/16 10:30:37  robertj
 * Added missing PThread::WaitForTermination function.
 *
 * Revision 1.12  1999/01/12 12:09:51  robertj
 * Removed redundent member variable, was in common.
 * Fixed BSD threads compatibility.
 *
 * Revision 1.11  1999/01/11 12:05:56  robertj
 * Fixed some more race conditions in threads.
 *
 * Revision 1.10  1999/01/11 03:42:26  robertj
 * Fixed problem with destroying thread automatically.
 *
 * Revision 1.9  1999/01/09 03:37:28  robertj
 * Fixed problem with closing thread waiting on semaphore.
 * Improved efficiency of mutex to use pthread functions directly.
 *
 * Revision 1.8  1999/01/08 01:31:03  robertj
 * Support for pthreads under FreeBSD
 *
 * Revision 1.7  1998/12/15 12:41:07  robertj
 * Fixed signal handling so can now ^C a pthread version.
 *
 * Revision 1.6  1998/11/05 09:45:04  robertj
 * Removed StartImmediate option in thread construction.
 *
 * Revision 1.5  1998/09/24 04:12:25  robertj
 * Added open software license.
 *
 */

#include <ptlib/socket.h>
#include <sched.h>	// for sched_yield
#include <pthread.h>
#include <sys/resource.h>

#ifdef P_RTEMS
#define	SUSPEND_SIG SIGALRM
#include <sched.h>
#else
#define	SUSPEND_SIG SIGVTALRM
#endif

#ifdef P_MACOSX
#include <mach/mach.h>
#include <mach/thread_policy.h>
#include <sys/param.h>
#include <sys/sysctl.h>
// going to need the main thread for adjusting relative priority
static pthread_t baseThread;
#endif

int PX_NewHandle(const char *, int);


#define PAssertPTHREAD(func, args) \
  { \
    unsigned threadOpRetry = 0; \
    while (PAssertThreadOp(func args, threadOpRetry, #func, __FILE__, __LINE__)); \
  }

static BOOL PAssertThreadOp(int retval,
                            unsigned & retry,
                            const char * funcname,
                            const char * file,
                            unsigned line)
{
  if (retval == 0) {
    PTRACE_IF(2, retry > 0, "PWLib\t" << funcname << " required " << retry << " retries!");
    return FALSE;
  }

  if (errno == EINTR || errno == EAGAIN) {
    if (++retry < 1000) {
#if defined(P_RTEMS)
      sched_yield();
#else
      usleep(10000); // Basically just swap out thread to try and clear blockage
#endif
      return TRUE;   // Return value to try again
    }
    // Give up and assert
  }

  PAssertFunc(file, line, NULL, psprintf("Function %s failed", funcname));
  return FALSE;
}


PDECLARE_CLASS(PHouseKeepingThread, PThread)
  public:
    PHouseKeepingThread()
      : PThread(1000, NoAutoDeleteThread, NormalPriority, "Housekeeper")
      { closing = FALSE; Resume(); }

    void Main();
    void SetClosing() { closing = TRUE; }

  protected:
    BOOL closing;
};


static pthread_mutex_t MutexInitialiser = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  CondInitialiser  = PTHREAD_COND_INITIALIZER;


#define new PNEW


void PHouseKeepingThread::Main()
{
  PProcess & process = PProcess::Current();

  while (!closing) {
    PTimeInterval delay = process.timers.Process();

    int fd = process.timerChangePipe[0];

    P_fd_set read_fds = fd;
    P_timeval tval = delay;
    if (::select(fd+1, read_fds, NULL, NULL, tval) == 1) {
      BYTE ch;
      ::read(fd, &ch, 1);
    }

    process.PXCheckSignals();
  }
}


void PProcess::SignalTimerChange()
{
  if (housekeepingThread == NULL) {
#if PMEMORY_CHECK
    BOOL oldIgnoreAllocations = PMemoryHeap::SetIgnoreAllocations(TRUE);
#endif
    housekeepingThread = new PHouseKeepingThread;
#if PMEMORY_CHECK
    PMemoryHeap::SetIgnoreAllocations(oldIgnoreAllocations);
#endif
  }

  BYTE ch;
  write(timerChangePipe[1], &ch, 1);
}


void PProcess::Construct()
{
#ifndef P_RTEMS
  // get the file descriptor limit
  struct rlimit rl;
  PAssertOS(getrlimit(RLIMIT_NOFILE, &rl) == 0);
  maxHandles = rl.rlim_cur;
  PTRACE(4, "PWLib\tMaximum per-process file handles is " << maxHandles);

  ::pipe(timerChangePipe);
#else
  maxHandles = 500; // arbitrary value
  socketpair(AF_INET,SOCK_STREAM,0,timerChangePipe);
#endif

  // initialise the housekeeping thread
  housekeepingThread = NULL;

#ifdef P_MACOSX
    // records the main thread for priority adjusting
    baseThread = pthread_self();
#endif

  CommonConstruct();
}


BOOL PProcess::SetMaxHandles(int newMax)
{
#ifndef P_RTEMS
  // get the current process limit
  struct rlimit rl;
  PAssertOS(getrlimit(RLIMIT_NOFILE, &rl) == 0);

  // set the new current limit
  rl.rlim_cur = newMax;
  if (setrlimit(RLIMIT_NOFILE, &rl) == 0) {
    PAssertOS(getrlimit(RLIMIT_NOFILE, &rl) == 0);
    maxHandles = rl.rlim_cur;
    if (maxHandles == newMax) {
      PTRACE(2, "PWLib\tNew maximum per-process file handles set to " << maxHandles);
      return TRUE;
    }
  }
#endif // !P_RTEMS

  PTRACE(1, "PWLib\tCannot set per-process file handle limit to "
         << newMax << " (is " << maxHandles << ") - check permissions");
  return FALSE;
}


PProcess::~PProcess()
{
  // Don't wait for housekeeper to stop if Terminate() is called from it.
  if (housekeepingThread != NULL && PThread::Current() != housekeepingThread) {
    housekeepingThread->SetClosing();
    SignalTimerChange();
    housekeepingThread->WaitForTermination();
    delete housekeepingThread;
  }
  CommonDestruct();
}


//////////////////////////////////////////////////////////////////////////////

PThread::PThread()
{
  // see InitialiseProcessThread()
}


void PThread::InitialiseProcessThread()
{
  autoDelete          = FALSE;

  PX_origStackSize    = 0;
  PX_threadId         = pthread_self();
  PX_priority         = NormalPriority;
  PX_suspendCount     = 0;

#ifndef P_HAS_SEMAPHORES
  PX_waitingSemaphore = NULL;
  PX_WaitSemMutex = MutexInitialiser;
#endif

  PX_suspendMutex = MutexInitialiser;

#ifdef P_RTEMS
  PAssertOS(socketpair(AF_INET,SOCK_STREAM,0,unblockPipe) == 0);
#else
  PAssertOS(::pipe(unblockPipe) == 0);
#endif

  ((PProcess *)this)->activeThreads.DisallowDeleteObjects();
  ((PProcess *)this)->activeThreads.SetAt((unsigned)PX_threadId, this);

  traceBlockIndentLevel = 0;
}


PThread::PThread(PINDEX stackSize,
                 AutoDeleteFlag deletion,
                 Priority priorityLevel,
                 const PString & name)
  : threadName(name)
{
  autoDelete = (deletion == AutoDeleteThread);

  PAssert(stackSize > 0, PInvalidParameter);
  PX_origStackSize = stackSize;
  PX_threadId = 0;
  PX_priority = priorityLevel;
  PX_suspendCount = 1;

#ifndef P_HAS_SEMAPHORES
  PX_waitingSemaphore = NULL;
  PX_WaitSemMutex = MutexInitialiser;
#endif

  PX_suspendMutex = MutexInitialiser;

#ifdef P_RTEMS
  PAssertOS(socketpair(AF_INET,SOCK_STREAM,0,unblockPipe) == 0);
#else
  PAssertOS(::pipe(unblockPipe) == 0);
#endif
  PX_NewHandle("Thread unblock pipe", PMAX(unblockPipe[0], unblockPipe[1]));

  // new thread is actually started the first time Resume() is called.
  PX_firstTimeStart = TRUE;

  traceBlockIndentLevel = 0;

  PTRACE(5, "PWLib\tCreated thread " << this << ' ' << threadName);
}


PThread::~PThread()
{
  if (PX_threadId != 0 && PX_threadId != pthread_self())
    Terminate();

  PAssertPTHREAD(::close, (unblockPipe[0]));
  PAssertPTHREAD(::close, (unblockPipe[1]));

#ifndef P_HAS_SEMAPHORES
  pthread_mutex_destroy(&PX_WaitSemMutex);
#endif

#ifdef P_NETBSD
  // If the mutex was not locked, the unlock will fail */
  pthread_mutex_trylock(&PX_suspendMutex);
#endif
  pthread_mutex_unlock(&PX_suspendMutex);
  pthread_mutex_destroy(&PX_suspendMutex);

  PTRACE(5, "PWLib\tDestroyed thread " << this << ' ' << threadName);
}


void PThread::Restart()
{
  if (!IsTerminated())
    return;

  pthread_attr_t threadAttr;
  pthread_attr_init(&threadAttr);
  pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

#if defined(P_LINUX)
  /*
    Set realtime scheduling if our effective user id is root (only then is this
    allowed) AND our priority is Highest.
      As far as I can see, we could use either SCHED_FIFO or SCHED_RR here, it
    doesn't matter.
      I don't know if other UNIX OSs have SCHED_FIFO and SCHED_RR as well.

    WARNING: a misbehaving thread (one that never blocks) started with Highest
    priority can hang the entire machine. That is why root permission is 
    neccessary.
  */
  if ((geteuid() == 0) && (PX_priority == HighestPriority))
    PAssertPTHREAD(pthread_attr_setschedpolicy, (&threadAttr, SCHED_FIFO));
#elif defined(P_RTEMS)
  pthread_attr_setstacksize(&threadAttr, 2*PTHREAD_MINIMUM_STACK_SIZE);
  pthread_attr_setinheritsched(&threadAttr, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&threadAttr, SCHED_OTHER);
  struct sched_param sched_param;
  sched_param.sched_priority = 125; /* set medium priority */
  pthread_attr_setschedparam(&threadAttr, &sched_param);
#endif

  PAssertPTHREAD(pthread_create, (&PX_threadId, &threadAttr, PX_ThreadStart, this));

#ifdef P_MACOSX
  if (PX_priority == HighestPriority) {
    PTRACE(1, "set thread to have the highest priority (MACOSX)");
    SetPriority(HighestPriority);
  }
#endif
}


void PX_SuspendSignalHandler(int)
{
  PThread * thread = PThread::Current();
  if (thread == NULL)
    return;

  BOOL notResumed = TRUE;
  while (notResumed) {
    BYTE ch;
    notResumed = ::read(thread->unblockPipe[0], &ch, 1) < 0 && errno == EINTR;
#if !( defined(P_NETBSD) && defined(P_NO_CANCEL) )
    pthread_testcancel();
#endif
  }
}


void PThread::Suspend(BOOL susp)
{
  PAssertPTHREAD(pthread_mutex_lock, (&PX_suspendMutex));

  // Check for start up condition, first time Resume() is called
  if (PX_firstTimeStart) {
    if (susp)
      PX_suspendCount++;
    else {
      if (PX_suspendCount > 0)
        PX_suspendCount--;
      if (PX_suspendCount == 0) {
        PX_firstTimeStart = FALSE;
        Restart();
      }
    }

    PAssertPTHREAD(pthread_mutex_unlock, (&PX_suspendMutex));
    return;
  }

#if defined(P_MACOSX) && (P_MACOSX <= 55)
  // Suspend - warn the user with an Assertion
  PAssertAlways("Cannot suspend threads on Mac OS X due to lack of pthread_kill()");
#else
  if (pthread_kill(PX_threadId, 0) == 0) {

    // if suspending, then see if already suspended
    if (susp) {
      PX_suspendCount++;
      if (PX_suspendCount == 1) {
        if (PX_threadId != pthread_self()) {
          signal(SUSPEND_SIG, PX_SuspendSignalHandler);
          pthread_kill(PX_threadId, SUSPEND_SIG);
        }
        else {
          PAssertPTHREAD(pthread_mutex_unlock, (&PX_suspendMutex));
          PX_SuspendSignalHandler(SUSPEND_SIG);
          return;  // Mutex already unlocked
        }
      }
    }

    // if resuming, then see if to really resume
    else if (PX_suspendCount > 0) {
      PX_suspendCount--;
      if (PX_suspendCount == 0) 
        PXAbortBlock();
    }
  }

  PAssertPTHREAD(pthread_mutex_unlock, (&PX_suspendMutex));
#endif // P_MACOSX
}


void PThread::Resume()
{
  Suspend(FALSE);
}


BOOL PThread::IsSuspended() const
{
  if (PX_firstTimeStart)
    return TRUE;

  if (IsTerminated())
    return FALSE;

  PAssertPTHREAD(pthread_mutex_lock, ((pthread_mutex_t *)&PX_suspendMutex));
  BOOL suspended = PX_suspendCount != 0;
  PAssertPTHREAD(pthread_mutex_unlock, ((pthread_mutex_t *)&PX_suspendMutex));
  return suspended;
}


void PThread::SetAutoDelete(AutoDeleteFlag deletion)
{
  PAssert(deletion != AutoDeleteThread || this != &PProcess::Current(), PLogicError);
  autoDelete = deletion == AutoDeleteThread;
}

#ifdef P_MACOSX
// obtain thread priority of the main thread
static unsigned long
GetThreadBasePriority ()
{
    thread_basic_info_data_t threadInfo;
    policy_info_data_t       thePolicyInfo;
    unsigned int             count;

    if (baseThread == 0) {
      return 0;
    }

    // get basic info
    count = THREAD_BASIC_INFO_COUNT;
    thread_info (pthread_mach_thread_np (baseThread), THREAD_BASIC_INFO,
		 (integer_t*)&threadInfo, &count);

    switch (threadInfo.policy) {
    case POLICY_TIMESHARE:
        count = POLICY_TIMESHARE_INFO_COUNT;
	thread_info(pthread_mach_thread_np (baseThread),
		    THREAD_SCHED_TIMESHARE_INFO,
		    (integer_t*)&(thePolicyInfo.ts), &count);
	return thePolicyInfo.ts.base_priority;
	break;

    case POLICY_FIFO:
        count = POLICY_FIFO_INFO_COUNT;
	thread_info(pthread_mach_thread_np (baseThread),
		    THREAD_SCHED_FIFO_INFO,
		    (integer_t*)&(thePolicyInfo.fifo), &count);
	if (thePolicyInfo.fifo.depressed) {
	    return thePolicyInfo.fifo.depress_priority;
	} else {
	    return thePolicyInfo.fifo.base_priority;
	}
	break;

    case POLICY_RR:
      count = POLICY_RR_INFO_COUNT;
      thread_info(pthread_mach_thread_np (baseThread),
		  THREAD_SCHED_RR_INFO,
		  (integer_t*)&(thePolicyInfo.rr), &count);
      if (thePolicyInfo.rr.depressed) {
	  return thePolicyInfo.rr.depress_priority;
      } else {
	  return thePolicyInfo.rr.base_priority;
      }
      break;
    }

    return 0;
}
#endif

void PThread::SetPriority(Priority priorityLevel)
{
  PX_priority = priorityLevel;

#if defined(P_LINUX)
  if (IsTerminated())
    return;

  struct sched_param sched_param;
  
  if ((priorityLevel == HighestPriority) && (geteuid() == 0) ) {
    sched_param.sched_priority = sched_get_priority_min( SCHED_FIFO );
    
    PAssertPTHREAD(pthread_setschedparam, (PX_threadId, SCHED_FIFO, &sched_param));
  }
  else if (priorityLevel != HighestPriority) {
    /* priority 0 is the only permitted value for the SCHED_OTHER scheduler */ 
    sched_param.sched_priority = 0;
    
    PAssertPTHREAD(pthread_setschedparam, (PX_threadId, SCHED_OTHER, &sched_param));
  }
#endif

#if defined(P_MACOSX)
  if (IsTerminated())
    return;

  if (priorityLevel == HighestPriority) {
    /* get fixed priority */
    {
      int result;

      thread_extended_policy_data_t   theFixedPolicy;
      thread_precedence_policy_data_t thePrecedencePolicy;
      long                            relativePriority;

      theFixedPolicy.timeshare = false; // set to true for a non-fixed thread
      result = thread_policy_set (pthread_mach_thread_np(PX_threadId),
				  THREAD_EXTENDED_POLICY,
				  (thread_policy_t)&theFixedPolicy,
				  THREAD_EXTENDED_POLICY_COUNT);
      if (result != KERN_SUCCESS) {
	PTRACE(1, "thread_policy - Couldn't set thread as fixed priority.");
      }

      // set priority

      // precedency policy's "importance" value is relative to
      // spawning thread's priority
      
      relativePriority = 62 - GetThreadBasePriority();
      PTRACE(1,  "relativePriority is " << relativePriority <<
	     " base priority is " << GetThreadBasePriority());
      
      thePrecedencePolicy.importance = relativePriority;
      result = thread_policy_set (pthread_mach_thread_np(PX_threadId),
				  THREAD_PRECEDENCE_POLICY,
				  (thread_policy_t)&thePrecedencePolicy, 
				  THREAD_PRECEDENCE_POLICY_COUNT);
      if (result != KERN_SUCCESS) {
	PTRACE(1, "thread_policy - Couldn't set thread priority.");
      }
    }
  }
#endif
}


PThread::Priority PThread::GetPriority() const
{
#if defined(LINUX)
  int schedulingPolicy;
  struct sched_param schedParams;
  
  PAssertPTHREAD(pthread_getschedparam, (PX_threadId, &schedulingPolicy, &schedParams));
  
  switch( schedulingPolicy )
  {
    case SCHED_OTHER:
      break;
      
    case SCHED_FIFO:
    case SCHED_RR:
      return HighestPriority;
      
    default:
      /* Unknown scheduler. We don't know what priority this thread has. */
      PTRACE(1, "PWLib\tPThread::GetPriority: unknown scheduling policy #" << schedulingPolicy);
  }
#endif

  return NormalPriority; /* as good a guess as any */
}


#ifndef P_HAS_SEMAPHORES
void PThread::PXSetWaitingSemaphore(PSemaphore * sem)
{
  PAssertPTHREAD(pthread_mutex_lock, (&PX_WaitSemMutex));
  PX_waitingSemaphore = sem;
  PAssertPTHREAD(pthread_mutex_unlock, (&PX_WaitSemMutex));
}
#endif


#ifdef P_GNU_PTH
// GNU PTH threads version (used by NetBSD)
// Taken from NetBSD pkg patches
void PThread::Sleep(const PTimeInterval & timeout)
{
  PTime lastTime;
  PTime targetTime = PTime() + timeout;

  sched_yield();
  lastTime = PTime();

  while (lastTime < targetTime) {
    P_timeval tval = targetTime - lastTime;
    if (select(0, NULL, NULL, NULL, tval) < 0 && errno != EINTR)
      break;

    pthread_testcancel();

    lastTime = PTime();
  }
}

#else
// Normal Posix threads version
void PThread::Sleep(const PTimeInterval & timeout)
{
  PTime lastTime;
  PTime targetTime = lastTime + timeout;
  do {
    P_timeval tval = targetTime - lastTime;
    if (select(0, NULL, NULL, NULL, tval) < 0 && errno != EINTR)
      break;

#if !( defined(P_NETBSD) && defined(P_NO_CANCEL) )
    pthread_testcancel();
#endif

    lastTime = PTime();
  } while (lastTime < targetTime);
}
#endif

void PThread::Yield()
{
  sched_yield();
}


PThread * PThread::Current()
{
  PProcess & process = PProcess::Current();
  process.threadMutex.Wait();
  PThread * thread = process.activeThreads.GetAt((unsigned)pthread_self());
  process.threadMutex.Signal();
  return thread;
}


void PThread::Terminate()
{
  if (PX_origStackSize <= 0)
    return;

  // don't use PThread::Current, as the thread may already not be in the
  // active threads list
  if (PX_threadId == pthread_self()) {
    pthread_exit(0);
    return;
  }

  if (IsTerminated())
    return;

  PTRACE(2, "PWLib\tForcing termination of thread " << (void *)this);

  PXAbortBlock();
  WaitForTermination(20);

#ifndef P_HAS_SEMAPHORES
  PAssertPTHREAD(pthread_mutex_lock, (&PX_WaitSemMutex));
  if (PX_waitingSemaphore != NULL) {
    PAssertPTHREAD(pthread_mutex_lock, (&PX_waitingSemaphore->mutex));
    PX_waitingSemaphore->queuedLocks--;
    PAssertPTHREAD(pthread_mutex_unlock, (&PX_waitingSemaphore->mutex));
    PX_waitingSemaphore = NULL;
  }
  PAssertPTHREAD(pthread_mutex_unlock, (&PX_WaitSemMutex));
#endif

#if ( defined(P_NETBSD) && defined(P_NO_CANCEL) )
  pthread_kill(PX_threadId,SIGKILL);
#else
  if (PX_threadId) {
    pthread_cancel(PX_threadId);
  }
#endif
}


BOOL PThread::IsTerminated() const
{
  if (PX_threadId == 0)
    return TRUE;

#if defined(P_MACOSX) && (P_MACOSX <= 55)
  // MacOS X (darwin 5.5) does not support pthread_kill so we cannot use it
  // to test the validity of the thread
#else
  if (pthread_kill(PX_threadId, 0) != 0)
    return TRUE;
#endif

  PTRACE(7, "PWLib\tIsTerminated(" << (void *)this << ") not dead yet");
  return FALSE;
}


void PThread::WaitForTermination() const
{
  PAssert(Current() != this, "Waiting for self termination!");
  
  while (!IsTerminated()) {
    Sleep(10); // sleep for 10ms. This slows down the busy loop removing 100%
               // CPU usage and also yeilds so other threads can run.
  } 
}


BOOL PThread::WaitForTermination(const PTimeInterval & maxWait) const
{
  PAssert(Current() != this, "Waiting for self termination!");
  
  PTRACE(6, "PWLib\tWaitForTermination(" << maxWait << ')');

  PTimer timeout = maxWait;
  while (!IsTerminated()) {
    if (timeout == 0)
      return FALSE;
    Sleep(10); // sleep for 10ms. This slows down the busy loop removing 100%
               // CPU usage and also yeilds so other threads can run.
  }
  return TRUE;
}


void * PThread::PX_ThreadStart(void * arg)
{ 
  pthread_t threadId = pthread_self();

  // self-detach
  pthread_detach(threadId);

  PThread * thread = (PThread *)arg;

  // Added this to guarantee that the thread creation (PThread::Restart)
  // has completed before we start the thread. Then the PX_threadId has
  // been set.
  pthread_mutex_lock(&thread->PX_suspendMutex);

  thread->SetThreadName(thread->GetThreadName());

  pthread_mutex_unlock(&thread->PX_suspendMutex);

  PProcess & process = PProcess::Current();

  PINDEX newHighWaterMark = 0;
  static PINDEX highWaterMark = 0;

  // add thread to thread list
  process.threadMutex.Wait();
  process.activeThreads.SetAt((unsigned)threadId, thread);
  if (process.activeThreads.GetSize() > highWaterMark)
    newHighWaterMark = highWaterMark = process.activeThreads.GetSize();
  process.threadMutex.Signal();

  PTRACE_IF(4, newHighWaterMark > 0, "PWLib\tThread high water mark set: " << newHighWaterMark);

  // make sure the cleanup routine is called when the thread exits
  pthread_cleanup_push(PThread::PX_ThreadEnd, arg);

  PTRACE(5, "PWLib\tStarted thread " << thread << ' ' << thread->threadName);

  // now call the the thread main routine
  thread->Main();

  // execute the cleanup routine
  pthread_cleanup_pop(1);

  return NULL;
}


void PThread::PX_ThreadEnd(void * arg)
{
  PThread * thread = (PThread *)arg;
  pthread_t id = thread->GetThreadId();
  if (id == 0) {
    // Don't know why, but pthreads under Linux at least can call this function
    // multiple times! Probably a bug, but we have to allow for it.
    PTRACE(2, "PWLib\tAttempted to multiply end thread " << thread << " ThreadID=" << (void *)id);
    return;
  }

  PTRACE(5, "PWLib\tEnded thread " << thread << ' ' << thread->threadName);

  // remove this thread from the active thread list
  PProcess & process = PProcess::Current();
  process.threadMutex.Wait();
  process.activeThreads.SetAt((unsigned)id, NULL);
  process.threadMutex.Signal();

  // delete the thread if required, note this is done this way to avoid
  // a race condition, the thread ID cannot be zeroed before the if!
  if (thread->autoDelete) {
    thread->PX_threadId = 0;  // Prevent terminating terminated thread

    // Now should be safe to delete the thread!
    delete thread;
  }
  else
    thread->PX_threadId = 0;
}


int PThread::PXBlockOnIO(int handle, int type, const PTimeInterval & timeout)
{
  PTRACE(7, "PWLib\tPThread::PXBlockOnIO(" << handle << ',' << type << ')');

  if ((handle < 0) || (handle >= PProcess::Current().GetMaxHandles())) {
    PTRACE(2, "PWLib\tAttempt to use illegal handle in PThread::PXBlockOnIO, handle=" << handle);
    errno = EBADF;
    return -1;
  }

  // make sure we flush the buffer before doing a write
  P_fd_set read_fds;
  P_fd_set write_fds;
  P_fd_set exception_fds;

  int retval;
  do {
    switch (type) {
      case PChannel::PXReadBlock:
      case PChannel::PXAcceptBlock:
        read_fds = handle;
        write_fds.Zero();
        exception_fds.Zero();
        break;
      case PChannel::PXWriteBlock:
        read_fds.Zero();
        write_fds = handle;
        exception_fds.Zero();
        break;
      case PChannel::PXConnectBlock:
        read_fds.Zero();
        write_fds = handle;
        exception_fds = handle;
        break;
      default:
        PAssertAlways(PLogicError);
        return 0;
    }

    // include the termination pipe into all blocking I/O functions
    read_fds += unblockPipe[0];

    P_timeval tval = timeout;
    retval = ::select(PMAX(handle, unblockPipe[0])+1,
                      read_fds, write_fds, exception_fds, tval);
  } while (retval < 0 && errno == EINTR);

  if ((retval == 1) && read_fds.IsPresent(unblockPipe[0])) {
    BYTE ch;
    ::read(unblockPipe[0], &ch, 1);
    errno = EINTR;
    retval =  -1;
    PTRACE(6, "PWLib\tUnblocked I/O");
  }

  return retval;
}

void PThread::PXAbortBlock() const
{
  BYTE ch;
  ::write(unblockPipe[1], &ch, 1);
}


///////////////////////////////////////////////////////////////////////////////

PSemaphore::PSemaphore(PXClass pxc)
{
  pxClass = pxc;
  mutex   = MutexInitialiser;
  condVar = CondInitialiser;

  // these should never be used, as this constructor is
  // only used for PMutex and PSyncPoint and they have their
  // own copy constructors
  initialVar = maxCountVar = 0;
}


PSemaphore::PSemaphore(unsigned initial, unsigned maxCount)
{
  pxClass = PXSemaphore;
  mutex   = MutexInitialiser;
  condVar = CondInitialiser;

  initialVar  = initial;
  maxCountVar = maxCount;

#ifdef P_HAS_SEMAPHORES
  PAssertPTHREAD(sem_init, (&semId, 0, initial));
#else
  PAssert(maxCount > 0, "Invalid semaphore maximum.");
  if (initial > maxCount)
    initial = maxCount;

  currentCount = initial;
  maximumCount = maxCount;
  queuedLocks  = 0;
#endif
}

PSemaphore::PSemaphore(const PSemaphore & sem)
{
  pxClass = sem.GetSemClass();
  mutex   = MutexInitialiser;
  condVar = CondInitialiser;

  initialVar  = sem.GetInitial();
  maxCountVar = sem.GetMaxCount();

#ifdef P_HAS_SEMAPHORES
  PAssertPTHREAD(sem_init, (&semId, 0, initialVar));
#else
  PAssert(maxCountVar > 0, "Invalid semaphore maximum.");
  if (initialVar > maxCountVar)
    initialVar = maxCountVar;

  currentCount = initialVar;
  maximumCount = maxCountVar;
  queuedLocks  = 0;
#endif
}

PSemaphore::~PSemaphore()
{
  pthread_cond_destroy(&condVar);

#ifdef P_NETBSD
  // If the mutex was not locked, the unlock will fail */
  pthread_mutex_trylock(&mutex);
#endif

  pthread_mutex_unlock(&mutex);
  pthread_mutex_destroy(&mutex);

  if (pxClass == PXSemaphore) {
#ifdef P_HAS_SEMAPHORES
    PAssertPTHREAD(sem_destroy, (&semId));
#else
    PAssert(queuedLocks == 0, "Semaphore destroyed with queued locks");
#endif
  }
}


void PSemaphore::Wait()
{
#ifdef P_HAS_SEMAPHORES
  PAssertPTHREAD(sem_wait, (&semId));
#else
  PAssertPTHREAD(pthread_mutex_lock, (&mutex));

  queuedLocks++;
  PThread::Current()->PXSetWaitingSemaphore(this);

  while (currentCount == 0) {
    int err = pthread_cond_wait(&condVar, &mutex);
    PAssert(err == 0 || err == EINTR, psprintf("wait error = %i", err));
  }

  PThread::Current()->PXSetWaitingSemaphore(NULL);
  queuedLocks--;

  currentCount--;

  PAssertPTHREAD(pthread_mutex_unlock, (&mutex));
#endif
}


BOOL PSemaphore::Wait(const PTimeInterval & waitTime)
{
  if (waitTime == PMaxTimeInterval) {
    Wait();
    return TRUE;
  }

  // create absolute finish time 
  PTime finishTime;
  finishTime += waitTime;

#ifdef P_HAS_SEMAPHORES

  // loop until timeout, or semaphore becomes available
  // don't use a PTimer, as this causes the housekeeping
  // thread to get very busy
  do {
    if (sem_trywait(&semId) == 0)
      return TRUE;

    PThread::Yield(); // One time slice
  } while (PTime() < finishTime);

  return FALSE;

#else

  struct timespec absTime;
  absTime.tv_sec  = finishTime.GetTimeInSeconds();
  absTime.tv_nsec = finishTime.GetMicrosecond() * 1000;

  PAssertPTHREAD(pthread_mutex_lock, (&mutex));

  PThread * thread = PThread::Current();
  thread->PXSetWaitingSemaphore(this);
  queuedLocks++;

  BOOL ok = TRUE;
  while (currentCount == 0) {
    int err = pthread_cond_timedwait(&condVar, &mutex, &absTime);
    if (err == ETIMEDOUT) {
      ok = FALSE;
      break;
    }
    else
      PAssert(err == 0 || err == EINTR, psprintf("timed wait error = %i", err));
  }

  thread->PXSetWaitingSemaphore(NULL);
  queuedLocks--;

  if (ok)
    currentCount--;

  PAssertPTHREAD(pthread_mutex_unlock, ((pthread_mutex_t *)&mutex));

  return ok;
#endif
}


void PSemaphore::Signal()
{
#ifdef P_HAS_SEMAPHORES
  PAssertPTHREAD(sem_post, (&semId));
#else
  PAssertPTHREAD(pthread_mutex_lock, (&mutex));

  if (currentCount < maximumCount)
    currentCount++;

  if (queuedLocks > 0) 
    PAssertPTHREAD(pthread_cond_signal, (&condVar));

  PAssertPTHREAD(pthread_mutex_unlock, (&mutex));
#endif
}


BOOL PSemaphore::WillBlock() const
{
#ifdef P_HAS_SEMAPHORES
  if (sem_trywait((sem_t *)&semId) != 0) {
    PAssertOS(errno == EAGAIN || errno == EINTR);
    return TRUE;
  }
  PAssertPTHREAD(sem_post, ((sem_t *)&semId));
  return FALSE;
#else
  return currentCount == 0;
#endif
}

#if defined(P_QNX) && defined(P_HAS_RECURSIVE_MUTEX)
#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
#endif

PMutex::PMutex()
  : PSemaphore(PXMutex)
{
#ifdef P_HAS_RECURSIVE_MUTEX
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
  pthread_mutex_init(&mutex, &attr);
#else
  ownerThreadId = (pthread_t)-1;
  lockCount = 0;
#endif
}

PMutex::PMutex(const PMutex & /*mut*/)
  : PSemaphore(PXMutex)
{
#ifdef P_HAS_RECURSIVE_MUTEX
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
  pthread_mutex_init(&mutex, &attr);
#else
  ownerThreadId = (pthread_t)-1;
  lockCount = 0;
#endif
}

void PMutex::Wait()
{
#ifndef P_HAS_RECURSIVE_MUTEX
  pthread_t currentThreadId = pthread_self();

  // if the mutex is already acquired by this thread,
  // then just increment the lock count
  if (pthread_equal(ownerThreadId, currentThreadId)) {
    // Note this does not need a lock as it can only be touched by the thread
    // which already has the mutex locked.
    lockCount++;
    return;
  }
#endif

  // acquire the lock for real
  PAssertPTHREAD(pthread_mutex_lock, (&mutex));

#ifndef P_HAS_RECURSIVE_MUTEX
  PAssert((ownerThreadId == (pthread_t)-1) && (lockCount == 0),
          "PMutex acquired whilst locked by another thread");
  // Note this is protected by the mutex itself only the thread with
  // the lock can alter it.
  ownerThreadId = currentThreadId;
#endif
}


BOOL PMutex::Wait(const PTimeInterval & waitTime)
{
  // if waiting indefinitely, then do so
  if (waitTime == PMaxTimeInterval) {
    Wait();
    return TRUE;
  }

#ifndef P_HAS_RECURSIVE_MUTEX
  // get the current thread ID
  pthread_t currentThreadId = pthread_self();

  // if we already have the mutex, return immediately
  if (pthread_equal(ownerThreadId, currentThreadId)) {
    // Note this does not need a lock as it can only be touched by the thread
    // which already has the mutex locked.
    lockCount++;
    return TRUE;
  }
#endif

  // create absolute finish time
  PTime finishTime;
  finishTime += waitTime;

  do {
    if (pthread_mutex_trylock(&mutex) == 0) {
#ifndef P_HAS_RECURSIVE_MUTEX
      PAssert((ownerThreadId == (pthread_t)-1) && (lockCount == 0),
              "PMutex acquired whilst locked by another thread");
      // Note this is protected by the mutex itself only the thread with
      // the lock can alter it.
      ownerThreadId = currentThreadId;
#endif

      return TRUE;
    }

    PThread::Current()->Sleep(10); // sleep for 10ms
  } while (PTime() < finishTime);

  return FALSE;
}


void PMutex::Signal()
{
#ifndef P_HAS_RECURSIVE_MUTEX
  if (!pthread_equal(ownerThreadId, pthread_self())) {
    PAssertAlways("PMutex signal failed - no matching wait or signal by wrong thread");
    return;
  }

  // if lock was recursively acquired, then decrement the counter
  // Note this does not need a separate lock as it can only be touched by the thread
  // which already has the mutex locked.
  if (lockCount > 0) {
    lockCount--;
    return;
  }

  // otherwise mark mutex as available
  ownerThreadId = (pthread_t)-1;
#endif

  PAssertPTHREAD(pthread_mutex_unlock, (&mutex));
#ifdef P_LINUX
  PThread::Yield();
#endif
}


BOOL PMutex::WillBlock() const
{
#ifndef P_HAS_RECURSIVE_MUTEX
  pthread_t currentThreadId = pthread_self();
  if (currentThreadId == ownerThreadId)
    return FALSE;
#endif

  pthread_mutex_t * mp = (pthread_mutex_t*)&mutex;
  if (pthread_mutex_trylock(mp) != 0)
    return TRUE;

  PAssertPTHREAD(pthread_mutex_unlock, (mp));
  return FALSE;
}


PSyncPoint::PSyncPoint()
  : PSemaphore(PXSyncPoint)
{
  signalCount = 0;
}

PSyncPoint::PSyncPoint(const PSyncPoint &)
  : PSemaphore(PXSyncPoint)
{
  signalCount = 0;
}

void PSyncPoint::Wait()
{
  PAssertPTHREAD(pthread_mutex_lock, (&mutex));
  while (signalCount == 0)
    pthread_cond_wait(&condVar, &mutex);
  signalCount--;
  PAssertPTHREAD(pthread_mutex_unlock, (&mutex));
}


BOOL PSyncPoint::Wait(const PTimeInterval & waitTime)
{
  PAssertPTHREAD(pthread_mutex_lock, (&mutex));

  PTime finishTime;
  finishTime += waitTime;
  struct timespec absTime;
  absTime.tv_sec  = finishTime.GetTimeInSeconds();
  absTime.tv_nsec = finishTime.GetMicrosecond() * 1000;

  int err = 0;
  while (signalCount == 0) {
    err = pthread_cond_timedwait(&condVar, &mutex, &absTime);
    if (err == 0 || err == ETIMEDOUT)
      break;

    PAssertOS(err == EINTR && errno == EINTR);
  }

  if (err == 0)
    signalCount--;

  PAssertPTHREAD(pthread_mutex_unlock, (&mutex));

  return err == 0;
}


void PSyncPoint::Signal()
{
  PAssertPTHREAD(pthread_mutex_lock, (&mutex));
  signalCount++;
  PAssertPTHREAD(pthread_cond_signal, (&condVar));
  PAssertPTHREAD(pthread_mutex_unlock, (&mutex));
}


BOOL PSyncPoint::WillBlock() const
{
  return signalCount == 0;
}


