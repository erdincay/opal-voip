/*
 * $Id: tcpsock.h,v 1.6 1995/01/01 01:07:33 robertj Exp $
 *
 * Portable Windows Library
 *
 * TCP Socket Class Declarations
 *
 * Copyright 1993 Equivalence
 *
 * $Log: tcpsock.h,v $
 * Revision 1.6  1995/01/01 01:07:33  robertj
 * More implementation.
 *
 * Revision 1.5  1994/08/23  11:32:52  robertj
 * Oops
 *
 * Revision 1.4  1994/08/22  00:46:48  robertj
 * Added pragma fro GNU C++ compiler.
 *
 * Revision 1.3  1994/08/21  23:43:02  robertj
 * Changed type of socket port number for better portability.
 * Added Out of Band data functions.
 *
 * Revision 1.2  1994/07/25  03:36:03  robertj
 * Added sockets to common, normalising to same comment standard.
 *
 */

#define _PTCPSOCKET

#ifdef __GNUC__
#pragma interface
#endif


PDECLARE_CLASS(PTCPSocket, PIPSocket)

  public:
    PTCPSocket(WORD port = 0);
      // Create an unassigned socket

    virtual BOOL Open(const PString & address, WORD port = 0);
      // Open a socket to a remote host on the specified port number

    virtual void OnOutOfBand(const void * buf, PINDEX len);
      // Read out of band data from the TCP/IP stream. This is subject to the
      // read timeout and sets the lastReadCount variable in the same way as
      // usual Read() function.

    virtual BOOL WriteOutOfBand(const void * buf, PINDEX len);
      // Write out of band data from the TCP/IP stream. This is subject to the
      // write timeout and sets the lastWriteCount variable in the same way as
      // usual Write() function.

    virtual void SetPort(WORD port);
    virtual void SetPort(const PString & service);

    virtual WORD GetPort() const;
    virtual PString GetService() const;

    virtual WORD    GetPort(const PString & service) const;
    virtual PString GetService(WORD port) const;

  protected:
    WORD port;

// Class declaration continued in platform specific header file ///////////////
