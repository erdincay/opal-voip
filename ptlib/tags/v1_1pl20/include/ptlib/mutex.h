/*
 * mutex.h
 *
 * Mutual exclusion thread synchonisation class.
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
 * $Log: mutex.h,v $
 * Revision 1.5  1999/03/09 02:59:50  robertj
 * Changed comments to doc++ compatible documentation.
 *
 * Revision 1.4  1999/02/16 08:12:22  robertj
 * MSVC 6.0 compatibility changes.
 *
 * Revision 1.3  1998/11/30 02:50:59  robertj
 * New directory structure
 *
 * Revision 1.2  1998/09/23 06:20:55  robertj
 * Added open source copyright license.
 *
 * Revision 1.1  1998/03/23 02:41:31  robertj
 * Initial revision
 *
 */


#define _PMUTEX

#ifdef __GNUC__
#pragma interface
#endif

#include <ptlib/semaphor.h>


/**This class defines a thread mutual exclusion object. A mutex is where a
   piece of code or data cannot be accessed by more than one thread at a time.
   To prevent this the PMutex is used in the following manner:
\begin{verbatim}
      PMutex mutex;

      ...

      mutex.Wait();

      ... critical section - only one thread at a time here.

      mutex.Signal();

      ...
\end{verbatim}
    The first thread will pass through the #Wait()# function, a second
    thread will block on that function until the first calls the
    #Signal()# function, releasing the second thread.
 */
class PMutex : public PSemaphore
{
  PCLASSINFO(PMutex, PSemaphore);

  public:
    /* Create a new mutex.
       Initially the mutex will not be "set", so the first call to Wait() will
       never wait.
     */
    PMutex();

#ifdef DOC_PLUS_PLUS
};
#endif

// Class declaration continued in platform specific header file ///////////////
