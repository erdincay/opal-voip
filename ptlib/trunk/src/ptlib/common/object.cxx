/*
 * object.cxx
 *
 * Global object support.
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
 * $Log: object.cxx,v $
 * Revision 1.69  2004/04/03 07:41:01  csoutheren
 * Fixed compile problem with ostringstream/ostrstream
 *
 * Revision 1.68  2004/04/03 06:54:28  rjongbloed
 * Many and various changes to support new Visual C++ 2003
 *
 * Revision 1.67  2003/09/17 09:02:14  csoutheren
 * Removed memory leak detection code
 *
 * Revision 1.66  2003/09/17 05:55:10  csoutheren
 * Removed recursive includes
 *
 * Revision 1.65  2003/02/11 08:05:55  robertj
 * Fixed problems in dumping memory leaks and statistics as PMemoryHeap
 *   is flagged deleted too early, thanks Diego T�rtara
 *
 * Revision 1.64  2003/02/06 21:03:13  dereks
 * Patch from Klaus Kaempf to fix warning message on compiling with gcc 3.x Thanks!
 *
 * Revision 1.63  2002/11/22 10:14:58  robertj
 * Fixed correct free of memory blocks in exceptional circumstances. Partially
 *   trashed heap or objects etc.
 *
 * Revision 1.62  2002/10/21 12:52:27  rogerh
 * Add throw()s to new and delete. Error reported by FreeBSD 5.0 and GCC 3.2.1
 *
 * Revision 1.61  2002/10/10 04:43:44  robertj
 * VxWorks port, thanks Martijn Roest
 *
 * Revision 1.60  2002/09/06 05:29:42  craigs
 * Reversed order of memory block check on delete to improve performance in
 * Linux debug mode
 *
 * Revision 1.59  2002/09/04 05:23:53  robertj
 * Fixed crashable way of checking pointer is in PWLib heap check.
 *
 * Revision 1.58  2002/08/13 01:28:42  robertj
 * Added allocation number to memory statistics dump to aid in leak finding.
 *
 * Revision 1.57  2002/06/28 05:17:06  yurik
 * ifndef'd ostrstream as it does not yet exist on wince
 *
 * Revision 1.56  2002/06/27 06:10:39  robertj
 * Removed PAssert on allocation breakpoint as can often caus a deadlock.
 *
 * Revision 1.55  2002/06/25 10:33:55  robertj
 * REmoved usage of sstream for GNU v3 as it is completely different to the
 *   old ostrstream classes.
 *
 * Revision 1.54  2002/06/25 02:23:57  robertj
 * Improved assertion system to allow C++ class name to be displayed if
 *   desired, especially relevant to container classes.
 *
 * Revision 1.53  2002/06/13 08:50:11  rogerh
 * GCC 3.1 uses slightly different #includes
 *
 * Revision 1.52  2001/09/18 05:56:03  robertj
 * Fixed numerous problems with thread suspend/resume and signals handling.
 *
 * Revision 1.51  2001/08/16 11:58:22  rogerh
 * Add more Mac OS X changes from John Woods <jfw@jfwhome.funhouse.com>
 *
 * Revision 1.50  2001/06/20 06:05:47  rogerh
 * Updates for Mac OS X from Pai-Hsiang Hsiao <shawn@eecs.harvard.edu>
 *
 * Revision 1.49  2001/05/03 06:27:29  robertj
 * Added return value to PMemoryCheck::SetIgnoreAllocations() so get previous state.
 *
 * Revision 1.48  2001/02/18 23:16:02  robertj
 * Fixed possible NULL pointer reference in memory check validation
 *   function, thanks Peter Ehlin.
 *
 * Revision 1.47  2001/02/13 03:31:02  robertj
 * Added function to do heap validation.
 *
 * Revision 1.46  2001/02/07 04:47:49  robertj
 * Added changes for possible random crashes in multi DLL environment
 *   due to memory allocation wierdness, thanks Milan Dimitrijevic.
 *
 * Revision 1.45  2001/01/25 07:14:39  robertj
 * Fixed spurios memory leak message. Usual static global problem.
 *
 * Revision 1.44  2000/06/26 11:17:20  robertj
 * Nucleus++ port (incomplete).
 *
 * Revision 1.43  2000/01/05 00:29:12  robertj
 * Fixed alignment problems in memory checking debug functions.
 *
 * Revision 1.42  1999/11/01 00:17:20  robertj
 * Added override of new functions for MSVC memory check code.
 *
 * Revision 1.41  1999/08/22 13:38:39  robertj
 * Fixed termination hang up problem with memory check code under unix pthreads.
 *
 * Revision 1.40  1999/08/10 10:45:09  robertj
 * Added mutex in memory check detection code.
 *
 * Revision 1.39  1999/07/18 15:08:48  robertj
 * Fixed 64 bit compatibility
 *
 * Revision 1.38  1999/05/01 11:29:20  robertj
 * Alpha linux port changes.
 *
 * Revision 1.37  1999/03/09 10:30:17  robertj
 * Fixed ability to have PMEMORY_CHECK on/off on both debug/release versions.
 *
 * Revision 1.36  1999/02/22 10:48:14  robertj
 * Fixed delete operator prototypes for MSVC6 and GNU compatibility.
 *
 * Revision 1.35  1998/12/22 10:24:17  robertj
 * Fixed MSVC warnings caused by changes made in linux PPC support.
 *
 * Revision 1.34  1998/12/15 09:01:10  robertj
 * Fixed 8 byte alignment problem in memory leak check code for sparc.
 *
 * Revision 1.33  1998/11/30 05:33:00  robertj
 * Fixed duplicate debug stream class, ther can be only one.
 *
 * Revision 1.32  1998/11/03 03:11:53  robertj
 * Fixed memory leak question so correctly detects leaks and can be ^C'd.
 *
 * Revision 1.31  1998/11/03 00:55:31  robertj
 * Added allocation breakpoint variable.
 *
 * Revision 1.30  1998/10/15 07:48:56  robertj
 * Added hex dump to memory leak.
 * Added ability to ignore G++lib memory leaks.
 *
 * Revision 1.29  1998/10/15 01:53:35  robertj
 * GNU compatibility.
 *
 * Revision 1.28  1998/10/13 14:06:26  robertj
 * Complete rewrite of memory leak detection code.
 *
 * Revision 1.27  1998/09/23 06:22:22  robertj
 * Added open source copyright license.
 *
 * Revision 1.26  1998/05/30 13:27:02  robertj
 * Changed memory check code so global statics are not included in leak check.
 *
 * Revision 1.25  1997/07/08 13:07:07  robertj
 * DLL support.
 *
 * Revision 1.24  1997/02/09 03:45:28  robertj
 * Fixed unix/dos compatibility with include file.
 *
 * Revision 1.23  1997/02/05 11:54:12  robertj
 * Fixed problems with memory check and leak detection.
 *
 * Revision 1.22  1996/08/08 10:08:46  robertj
 * Directory structure changes for common files.
 *
 * Revision 1.21  1996/07/15 10:35:11  robertj
 * Changed memory leak dump to use static class rather than atexit for better portability.
 *
 * Revision 1.20  1996/06/17 11:35:47  robertj
 * Fixed display of memory leak info, needed flush and use of cin as getchar() does not work with services.
 *
 * Revision 1.19  1996/05/09 12:19:29  robertj
 * Fixed up 64 bit integer class for Mac platform.
 * Fixed incorrect use of memcmp/strcmp return value.
 *
 * Revision 1.18  1996/03/26 00:55:20  robertj
 * Added keypress before dumping memory leaks.
 *
 * Revision 1.17  1996/01/28 02:50:27  robertj
 * Added missing bit shift operators to 64 bit integer class.
 * Added assert into all Compare functions to assure comparison between compatible objects.
 *
 * Revision 1.16  1996/01/23 13:15:52  robertj
 * Mac Metrowerks compiler support.
 *
 * Revision 1.15  1996/01/02 12:52:02  robertj
 * Mac OS compatibility changes.
 *
 * Revision 1.14  1995/11/21 11:51:54  robertj
 * Improved streams compatibility.
 *
 * Revision 1.12  1995/04/25 11:30:34  robertj
 * Fixed Borland compiler warnings.
 * Fixed function hiding ancestors virtual.
 *
 * Revision 1.11  1995/03/12 04:59:53  robertj
 * Re-organisation of DOS/WIN16 and WIN32 platforms to maximise common code.
 * Used built-in equate for WIN32 API (_WIN32).
 *
 * Revision 1.10  1995/02/19  04:19:21  robertj
 * Added dynamically linked command processing.
 *
 * Revision 1.9  1995/01/15  04:52:02  robertj
 * Mac compatibility.
 * Added memory stats function.
 *
// Revision 1.8  1995/01/09  12:38:07  robertj
// Changed variable names around during documentation run.
// Fixed smart pointer comparison.
// Fixed serialisation stuff.
//
// Revision 1.7  1995/01/07  04:39:45  robertj
// Redesigned font enumeration code and changed font styles.
//
// Revision 1.6  1995/01/04  10:57:08  robertj
// Changed for HPUX and GNU2.6.x
//
// Revision 1.5  1995/01/03  09:39:10  robertj
// Put standard malloc style memory allocation etc into memory check system.
//
// Revision 1.4  1994/12/21  11:43:29  robertj
// Added extra memory stats.
//
// Revision 1.3  1994/12/13  11:54:54  robertj
// Added some memory usage statistics.
//
// Revision 1.2  1994/12/12  10:08:32  robertj
// Renamed PWrapper to PSmartPointer..
//
// Revision 1.1  1994/10/30  12:02:15  robertj
// Initial revision
//
 */

#include <ptlib.h>
#include <ctype.h>
#ifdef _WIN32
#include <ptlib/msos/ptlib/debstrm.h>
#include <crtdbg.h>
#elif defined(__NUCLEUS_PLUS__)
#include <ptlib/NucleusDebstrm.h>
#else
#include <signal.h>
#endif


void PAssertFunc(const char * file,
                 int line,
                 const char * className,
                 PStandardAssertMessage msg)
{
  if (msg == POutOfMemory) {
    // Special case, do not use ostrstream in other PAssertFunc if have
    // a memory out situation as that would probably also fail!
    static const char fmt[] = "Out of memory at file %.100s, line %u, class %.30s";
    char msgbuf[sizeof(fmt)+100+10+30];
    sprintf(msgbuf, fmt, file, line, className);
    PAssertFunc(msgbuf);
    return;
  }

  static const char * const textmsg[PMaxStandardAssertMessage] = {
    NULL,
    "Out of memory",
    "Null pointer reference",
    "Invalid cast to non-descendant class",
    "Invalid array index",
    "Invalid array element",
    "Stack empty",
    "Unimplemented function",
    "Invalid parameter",
    "Operating System error",
    "File not open",
    "Unsupported feature",
    "Invalid or closed operating system window"
  };

  const char * theMsg;
  char msgbuf[20];
  if (msg < PMaxStandardAssertMessage)
    theMsg = textmsg[msg];
  else {
    sprintf(msgbuf, "Assertion %i", msg);
    theMsg = msgbuf;
  }
  PAssertFunc(file, line, className, theMsg);
}


void PAssertFunc(const char * file, int line, const char * className, const char * msg)
{
#if defined(_WIN32)
  DWORD err = GetLastError();
#else
  int err = errno;
#endif

#ifndef _WIN32_WCE
  #if (__GNUC__ >= 3)
  ostringstream str;
  #else
  ostrstream str;
  #endif
  str << "Assertion fail: ";
  if (msg != NULL)
    str << msg << ", ";
  str << "file " << file << ", line " << line;
  if (className != NULL)
    str << ", class " << className;
  if (err != 0)
    str << ", Error=" << err;
  str << ends;

  PAssertFunc(str.str().c_str());
#endif // !_WIN32_WCE
}

const char * PObject::GetClass(unsigned) const
{
  return Class();
}


BOOL PObject::IsClass(const char * clsName) const
{
  return strcmp(clsName, Class()) == 0;
}


BOOL PObject::IsDescendant(const char * clsName) const
{
  return strcmp(clsName, Class()) == 0;
}


PObject::Comparison PObject::CompareObjectMemoryDirect(const PObject&obj) const
{
  int retval = memcmp(this, &obj, sizeof(PObject));
  if (retval < 0)
    return LessThan;
  if (retval > 0)
    return GreaterThan;
  return EqualTo;
}


PObject * PObject::Clone() const
{
  PAssertAlways(PUnimplementedFunction);
  return NULL;
}


PObject::Comparison PObject::Compare(const PObject & obj) const
{
  return (Comparison)CompareObjectMemoryDirect(obj);
}


void PObject::PrintOn(ostream & strm) const
{
  strm << GetClass();
}


void PObject::ReadFrom(istream &)
{
}


PINDEX PObject::HashFunction() const
{
  return 0;
}


///////////////////////////////////////////////////////////////////////////////
// General reference counting support

PSmartPointer::PSmartPointer(const PSmartPointer & ptr)
{
  object = ptr.object;
  if (object != NULL)
    ++object->referenceCount;
}


PSmartPointer & PSmartPointer::operator=(const PSmartPointer & ptr)
{
  if (object == ptr.object)
    return *this;

  if (object != NULL && --object->referenceCount == 0)
    delete object;

  object = ptr.object;
  if (object != NULL)
    ++object->referenceCount;

  return *this;
}


PSmartPointer::~PSmartPointer()
{
  if (object != NULL && --object->referenceCount == 0)
    delete object;
}


PObject::Comparison PSmartPointer::Compare(const PObject & obj) const
{
  PAssert(obj.IsDescendant(PSmartPointer::Class()), PInvalidCast);
  PSmartObject * other = ((const PSmartPointer &)obj).object;
  if (object == other)
    return EqualTo;
  return object < other ? LessThan : GreaterThan;
}


///////////////////////////////////////////////////////////////////////////////
// Large integer support

#ifdef P_NEEDS_INT64

void PInt64__::Add(const PInt64__ & v)
{
  unsigned long old = low;
  high += v.high;
  low += v.low;
  if (low < old)
    high++;
}


void PInt64__::Sub(const PInt64__ & v)
{
  unsigned long old = low;
  high -= v.high;
  low -= v.low;
  if (low > old)
    high--;
}


void PInt64__::Mul(const PInt64__ & v)
{
  DWORD p1 = (low&0xffff)*(v.low&0xffff);
  DWORD p2 = (low >> 16)*(v.low >> 16);
  DWORD p3 = (high&0xffff)*(v.high&0xffff);
  DWORD p4 = (high >> 16)*(v.high >> 16);
  low = p1 + (p2 << 16);
  high = (p2 >> 16) + p3 + (p4 << 16);
}


void PInt64__::Div(const PInt64__ & v)
{
  long double dividend = high;
  dividend *=  4294967296.0;
  dividend += low;
  long double divisor = high;
  divisor *=  4294967296.0;
  divisor += low;
  long double quotient = dividend/divisor;
  low = quotient;
  high = quotient/4294967296.0;
}


void PInt64__::Mod(const PInt64__ & v)
{
  PInt64__ t = *this;
  t.Div(v);
  t.Mul(t);
  Sub(t);
}


void PInt64__::ShiftLeft(int bits)
{
  if (bits >= 32) {
    high = low << (bits - 32);
    low = 0;
  }
  else {
    high <<= bits;
    high |= low >> (32 - bits);
    low <<= bits;
  }
}


void PInt64__::ShiftRight(int bits)
{
  if (bits >= 32) {
    low = high >> (bits - 32);
    high = 0;
  }
  else {
    low >>= bits;
    low |= high << (32 - bits);
    high >>= bits;
  }
}


BOOL PInt64::Lt(const PInt64 & v) const
{
  if ((long)high < (long)v.high)
    return TRUE;
  if ((long)high > (long)v.high)
    return FALSE;
  if ((long)high < 0)
    return (long)low > (long)v.low;
  return (long)low < (long)v.low;
}


BOOL PInt64::Gt(const PInt64 & v) const
{
  if ((long)high > (long)v.high)
    return TRUE;
  if ((long)high < (long)v.high)
    return FALSE;
  if ((long)high < 0)
    return (long)low < (long)v.low;
  return (long)low > (long)v.low;
}


BOOL PUInt64::Lt(const PUInt64 & v) const
{
  if (high < v.high)
    return TRUE;
  if (high > v.high)
    return FALSE;
  return low < high;
}


BOOL PUInt64::Gt(const PUInt64 & v) const
{
  if (high > v.high)
    return TRUE;
  if (high < v.high)
    return FALSE;
  return low > high;
}


static void Out64(ostream & stream, PUInt64 num)
{
  char buf[25];
  char * p = &buf[sizeof(buf)];
  *--p = '\0';

  switch (stream.flags()&ios::basefield) {
    case ios::oct :
      while (num != 0) {
        *--p = (num&7) + '0';
        num >>= 3;
      }
      break;

    case ios::hex :
      while (num != 0) {
        *--p = (num&15) + '0';
        if (*p > '9')
          *p += 7;
        num >>= 4;
      }
      break;

    default :
      while (num != 0) {
        *--p = num%10 + '0';
        num /= 10;
      }
  }

  if (*p == '\0')
    *--p = '0';

  stream << p;
}


ostream & operator<<(ostream & stream, const PInt64 & v)
{
  if (v >= 0)
    Out64(stream, v);
  else {
    int w = stream.width();
    stream.put('-');
    if (w > 0)
      stream.width(w-1);
    Out64(stream, -v);
  }

  return stream;
}


ostream & operator<<(ostream & stream, const PUInt64 & v)
{
  Out64(stream, v);
  return stream;
}


static PUInt64 Inp64(istream & stream)
{
  int base;
  switch (stream.flags()&ios::basefield) {
    case ios::oct :
      base = 8;
      break;
    case ios::hex :
      base = 16;
      break;
    default :
      base = 10;
  }

  if (isspace(stream.peek()))
    stream.get();

  PInt64 num = 0;
  while (isxdigit(stream.peek())) {
    int c = stream.get() - '0';
    if (c > 9)
      c -= 7;
    if (c > 9)
      c -= 32;
    num = num*base + c;
  }

  return num;
}


istream & operator>>(istream & stream, PInt64 & v)
{
  if (isspace(stream.peek()))
    stream.get();

  switch (stream.peek()) {
    case '-' :
      stream.ignore();
      v = -(PInt64)Inp64(stream);
      break;
    case '+' :
      stream.ignore();
    default :
      v = (PInt64)Inp64(stream);
  }

  return stream;
}


istream & operator>>(istream & stream, PUInt64 & v)
{
  v = Inp64(stream);
  return stream;
}


#endif


#ifdef P_TORNADO

// the library provided with Tornado 2.0 does not contain implementation 
// for the functions defined below, therefor the own implementation

ostream & ostream::operator<<(PInt64 v)
{
  return *this << (long)(v >> 32) << (long)(v & 0xFFFFFFFF);
}


ostream & ostream::operator<<(PUInt64 v)
{
  return *this << (long)(v >> 32) << (long)(v & 0xFFFFFFFF);
}

istream & istream::operator>>(PInt64 & v)
{
  return *this >> (long)(v >> 32) >> (long)(v & 0xFFFFFFFF);
}


istream & istream::operator>>(PUInt64 & v)
{
  return *this >> (long)(v >> 32) >> (long)(v & 0xFFFFFFFF);
}

#endif // P_TORNADO


// End Of File ///////////////////////////////////////////////////////////////
