/*
 * udpsock.h
 *
 * User Datagram Protocol socket I/O channel class.
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
 * $Log: udpsock.h,v $
 * Revision 1.19  2002/10/08 12:41:51  robertj
 * Changed for IPv6 support, thanks Sébastien Josset.
 *
 * Revision 1.18  2002/09/16 01:08:59  robertj
 * Added #define so can select if #pragma interface/implementation is used on
 *   platform basis (eg MacOS) rather than compiler, thanks Robert Monaghan.
 *
 * Revision 1.17  2001/05/22 12:49:32  robertj
 * Did some seriously wierd rewrite of platform headers to eliminate the
 *   stupid GNU compiler warning about braces not matching.
 *
 * Revision 1.16  1999/08/27 08:18:52  robertj
 * Added ability to get the host/port of the the last packet read/written to UDP socket.
 *
 * Revision 1.15  1999/03/09 02:59:51  robertj
 * Changed comments to doc++ compatible documentation.
 *
 * Revision 1.14  1999/02/16 08:11:17  robertj
 * MSVC 6.0 compatibility changes.
 *
 * Revision 1.13  1998/09/23 06:21:47  robertj
 * Added open source copyright license.
 *
 * Revision 1.12  1997/06/06 10:54:11  craigs
 * Added overrides and new functions for connectionless Writes
 *
 * Revision 1.11  1996/09/14 13:09:43  robertj
 * Major upgrade:
 *   rearranged sockets to help support IPX.
 *   added indirect channel class and moved all protocols to descend from it,
 *   separating the protocol from the low level byte transport.
 *
 * Revision 1.10  1996/05/15 10:19:15  robertj
 * Added ICMP protocol socket, getting common ancestor to UDP.
 *
 * Revision 1.9  1996/03/03 07:38:00  robertj
 * Added Reusability clause to the Listen() function on sockets.
 *
 * Revision 1.8  1995/12/10 11:44:45  robertj
 * Numerous fixes for sockets.
 *
 * Revision 1.7  1995/06/17 11:13:41  robertj
 * Documentation update.
 *
 * Revision 1.6  1995/06/17 00:48:01  robertj
 * Implementation.
 *
 * Revision 1.5  1995/01/03 09:36:24  robertj
 * Documentation.
 *
 * Revision 1.4  1994/08/23  11:32:52  robertj
 * Oops
 *
 * Revision 1.3  1994/08/22  00:46:48  robertj
 * Added pragma fro GNU C++ compiler.
 *
 * Revision 1.2  1994/07/25  03:36:03  robertj
 * Added sockets to common, normalising to same comment standard.
 *
 */

#define _PUDPSOCKET

#ifdef P_USE_PRAGMA
#pragma interface
#endif

/**
   A socket channel that uses the UDP transport on the Internet Protocol.
 */
class PUDPSocket : public PIPDatagramSocket
{
  PCLASSINFO(PUDPSocket, PIPDatagramSocket);

  public:
  /**@name Construction */
  //@{
    /** Create a UDP socket. If a remote machine address or
       a "listening" socket is specified then the channel is also opened.
     */
    PUDPSocket(
      WORD port = 0             /// Port number to use for the connection.
    );
    PUDPSocket(
      const PString & service   /// Service name to use for the connection.
    );
    PUDPSocket(
      const PString & address,  /// Address of remote machine to connect to.
      WORD port                 /// Port number to use for the connection.
    );
    PUDPSocket(
      const PString & address,  /// Address of remote machine to connect to.
      const PString & service   /// Service name to use for the connection.
    );
  //@}

  /**@name Overrides from class PSocket */
  //@{
    /** Override of PChannel functions to allow connectionless reads
     */
    BOOL Read(
      void * buf,   /// Pointer to a block of memory to read.
      PINDEX len    /// Number of bytes to read.
    );

    /** Override of PChannel functions to allow connectionless writes
     */
    BOOL Write(
      const void * buf, /// Pointer to a block of memory to write.
      PINDEX len        /// Number of bytes to write.
    );

    /** Override of PSocket functions to allow connectionless writes
     */
    BOOL Connect(
      const PString & address   /// Address of remote machine to connect to.
    );
  //@}

  /**@name New functions for class */
  //@{
    /** Set the address to use for connectionless Write().
     */
    void SetSendAddress(
      const Address & address,    /// IP address to send packets.
      WORD port                   /// Port to send packets.
    );

    /** Get the address to use for connectionless Write().
     */
    void GetSendAddress(
      Address & address,    /// IP address to send packets.
      WORD & port           /// Port to send packets.
    );

    /** Get the address of the sender in the last connectionless Read().
        Note that thsi only applies to the Read() and not the ReadFrom()
        function.
     */
    void GetLastReceiveAddress(
      Address & address,    /// IP address to send packets.
      WORD & port           /// Port to send packets.
    );
  //@}

  protected:
    // Open an IPv4 socket (for backward compatibility)
    virtual BOOL OpenSocket();

    // Open an IPv4 or IPv6 socket
    virtual BOOL OpenSocket(
      int ipAdressFamily
    );

    virtual const char * GetProtocolName() const;

    Address sendAddress;
    WORD    sendPort;

    Address lastReceiveAddress;
    WORD    lastReceivePort;


// Include platform dependent part of class
#include <ptlib/udpsock.h>
};


// End Of File ///////////////////////////////////////////////////////////////
