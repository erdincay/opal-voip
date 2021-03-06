/*
 * critsec.h
 *
 * Critical section mutex class.
 *
 * Portable Windows Library
 *
 * Copyright (C) 2004 Post Increment
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
 * The Initial Developer of the Original Code is Post Increment
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: critsec.h,v $
 * Revision 1.2  2004/04/18 12:37:40  csoutheren
 * Modified to detect sem_wait etc on Linux systems
 *
 * Revision 1.1  2004/04/11 03:02:07  csoutheren
 * Initial version
 *
 */

  // Unix specific critical section implementation
#ifdef P_HAS_SEMAPHORES
  sem_t sem;
#endif

// End Of File ///////////////////////////////////////////////////////////////
