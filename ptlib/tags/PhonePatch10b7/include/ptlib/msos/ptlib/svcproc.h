/*
 * $Id: svcproc.h,v 1.21 1998/04/07 13:32:31 robertj Exp $
 *
 * Portable Windows Library
 *
 * Service Process for Windows NT platforms
 *
 * Copyright 1995 Equivalence
 *
 * $Log: svcproc.h,v $
 * Revision 1.21  1998/04/07 13:32:31  robertj
 * Changed startup code to support PApplication class.
 *
 * Revision 1.20  1998/03/29 06:16:50  robertj
 * Rearranged initialisation sequence so PProcess descendent constructors can do "things".
 *
 * Revision 1.19  1997/07/08 13:00:57  robertj
 * DLL support.
 *
 * Revision 1.18  1997/03/18 21:23:26  robertj
 * Fix service manager falsely accusing app of crashing if OnStart() is slow.
 *
 * Revision 1.17  1997/02/05 11:49:38  robertj
 * Changed current process function to return reference and validate objects descendancy.
 *
 * Revision 1.16  1996/11/18 11:32:02  robertj
 * Fixed bug in doing a "stop" command closing ALL instances of service.
 *
 * Revision 1.15  1996/10/31 12:38:56  robertj
 * Fixed bug in window not being displayed when command line used.
 *
 * Revision 1.14  1996/10/08 13:04:53  robertj
 * Rewrite to use standard window isntead of console window.
 *
 * Revision 1.13  1996/08/19 13:36:41  robertj
 * Moved PSYSTEMLOG macro to common code.
 *
 * Revision 1.12  1996/08/17 10:00:35  robertj
 * Changes for Windows DLL support.
 *
 * Revision 1.11  1996/08/09 11:17:26  robertj
 * Moved log macro to platform dependent header.
 *
 * Revision 1.10  1996/08/08 10:09:16  robertj
 * Directory structure changes for common files.
 *
 * Revision 1.9  1996/07/30 12:23:44  robertj
 * Changed SIGINTR handler to just set termination event.
 *
 * Revision 1.8  1996/07/20 05:33:06  robertj
 * Fixed some Win95 service bugs and missing functionality (service stop).
 *
 * Revision 1.7  1996/05/23 10:03:00  robertj
 * Windows 95 support.
 *
 * Revision 1.6  1996/04/17 12:09:02  robertj
 * Added service dependencies.
 *
 * Revision 1.5  1996/01/28 02:55:02  robertj
 * WIN16 support.
 *
 * Revision 1.4  1996/01/02 12:54:12  robertj
 * Made "common".
 *
 * Revision 1.3  1995/12/10 11:50:05  robertj
 * Numerous fixes for WIN32 service processes.
 *
 * Revision 1.2  1995/07/02 01:23:27  robertj
 * Set up service process to be in subthread not main thread.
 *
 * Revision 1.1  1995/06/17 00:50:54  robertj
 * Initial revision
 *
 */


#ifndef _PSERVICEPROCESS


///////////////////////////////////////////////////////////////////////////////
// PServiceProcess

#include "../../common/ptlib/svcproc.h"
#ifdef _WIN32
  public:
    virtual const char * GetServiceDependencies() const { return NULL; }
      // Get a set of null terminated strings terminated with double null.

  private:
    static void __stdcall StaticMainEntry(DWORD argc, LPTSTR * argv);
    /* Internal function called from the Service Manager. This simply calls the
       <A>MainEntry()</A> function on the PServiceProcess instance.
    */

    void MainEntry(DWORD argc, LPTSTR * argv);
    /* Internal function function that takes care of actually starting the
       service, informing the service controller at each step along the way.
       After launching the worker thread, it waits on the event that the worker
       thread will signal at its termination.
    */

    static void StaticThreadEntry(void *);
    /* Internal function called to begin the work of the service process. This
       essentially just calls the <A>Main()</A> function on the
       PServiceProcess instance.
    */

    void ThreadEntry();
    /* Internal function function that starts the worker thread for the
       service.
    */

    static void __stdcall StaticControlEntry(DWORD code);
    /* This function is called by the Service Controller whenever someone calls
       ControlService in reference to our service.
     */

    void ControlEntry(DWORD code);
    /* This function is called by the Service Controller whenever someone calls
       ControlService in reference to our service.
     */

    static void Control_C(int);
    /* This function is called on a SIGINTR (Control-C) signal for use in
       debug mode.
     */

    BOOL ReportStatus(
      DWORD dwCurrentState,
      DWORD dwWin32ExitCode = NO_ERROR,
      DWORD dwCheckPoint = 0,
      DWORD dwWaitHint = 0
    );
    /* This function is called by the Main() and Control() functions to update the
       service's status to the service control manager.
     */


    BOOL ProcessCommand(const char * cmd);
    // Process command line argument for controlling the service.

    BOOL CreateControlWindow(BOOL createDebugWindow);
    static LPARAM WINAPI StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LPARAM WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void DebugOutput(const char * out);

    BOOL                  isWin95;
    SERVICE_STATUS        status;
    SERVICE_STATUS_HANDLE statusHandle;
    HANDLE                startedEvent;
    HANDLE                terminationEvent;
    HWND                  controlWindow;
    HWND                  debugWindow;

#endif

  friend void PAssertFunc(const char * file, int line, const char * msg);
};

#if defined(_MSC_VER) && !defined(_WIN32)
extern "C" int __argc;
extern "C" char ** __argv;
#endif

#ifdef __BORLANDC__
#define __argc _argc
#define __argv _argv
#endif

#undef PCREATE_PROCESS
#define PCREATE_PROCESS(cls) \
  extern "C" int PASCAL WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) \
    { PProcess::PreInitialise(__argc, __argv, _environ); \
      static cls instance; \
      return instance._main(hInst); \
    }


#endif
