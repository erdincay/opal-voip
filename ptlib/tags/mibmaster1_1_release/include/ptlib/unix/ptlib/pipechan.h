/*
 * $Id: pipechan.h,v 1.2 1996/04/15 10:50:48 craigs Exp $
 *
 * Portable Windows Library
 *
 * User Interface Classes Interface Declarations
 *
 * Copyright 1993 Equivalence
 *
 * $Log: pipechan.h,v $
 * Revision 1.2  1996/04/15 10:50:48  craigs
 * Last revision prior to release of MibMaster
 *
 * Revision 1.1  1995/01/23 18:43:27  craigs
 * Initial revision
 *
 */

#ifndef _PPIPECHANNEL

#pragma interface

#include <signal.h>

#include "../../common/pipechan.h"
  public:
    void PXWaitForTerminate();
    void PXKill(int killType = SIGTERM);
    
  protected:
    int toChildPipe[2];
    int fromChildPipe[2];
    int childPid;
    int retVal;
};

#ifdef P_USE_INLINES
#include "../../common/pipechan.inl"
#endif

#endif
