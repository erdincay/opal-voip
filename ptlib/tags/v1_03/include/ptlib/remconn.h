/*
 * remconn.h
 *
 * Remote networking connection class.
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
 * $Log: remconn.h,v $
 * Revision 1.12  1999/02/16 08:11:10  robertj
 * MSVC 6.0 compatibility changes.
 *
 * Revision 1.11  1998/09/23 06:21:17  robertj
 * Added open source copyright license.
 *
 * Revision 1.10  1998/07/24 06:58:27  robertj
 * Added ability to get IP number of RAS connection.
 *
 * Revision 1.9  1998/02/03 06:28:46  robertj
 * Added more error codes.
 *
 * Revision 1.8  1998/01/26 00:34:51  robertj
 * Added parameter to PRemoteConnection to open only if already connected.
 * Added function to PRemoteConnection to get at OS error code.
 *
 * Revision 1.7  1997/04/01 06:00:05  robertj
 * Added Remove Configuration.
 *
 * Revision 1.6  1997/01/12 04:15:11  robertj
 * Added ability to add/change new connections.
 *
 * Revision 1.5  1996/11/04 03:40:43  robertj
 * Added more debugging for remote drop outs.
 *
 * Revision 1.4  1996/08/11 07:03:45  robertj
 * Changed remote connection to late bind DLL.
 *
 * Revision 1.3  1996/04/23 11:33:04  robertj
 * Added username and password.
 *
 * Revision 1.2  1996/03/02 03:09:48  robertj
 * Added function to get all possible remote access connection names.
 *
 * Revision 1.1  1995/12/10 13:04:46  robertj
 * Initial revision
 *
 */

#define _PREMOTECONNECTION

#ifdef __GNUC__
#pragma interface
#endif


class PRemoteConnection : public PObject
{
  PCLASSINFO(PRemoteConnection, PObject)
  public:
    PRemoteConnection();
    PRemoteConnection(
      const PString & name
    );
    // Create a new remote connection.

    ~PRemoteConnection();

    virtual Comparison Compare(const PObject & obj) const;
    virtual PINDEX HashFunction() const;

    BOOL Open(BOOL existing = FALSE);
    BOOL Open(
      const PString & name,
      BOOL existing = FALSE
    );
    BOOL Open(
      const PString & name,
      const PString & username,
      const PString & password,
      BOOL existing = FALSE
    );
    void Close();

    const PString & GetName() const { return remoteName; }
    /* Get the name of the RAS connection.

       <H2>Returns:</H2>
       String for IP address, or empty string if none.
     */

    enum Status {
      Idle,
      Connected,
      InProgress,
      LineBusy,
      NoDialTone,
      NoAnswer,
      PortInUse,
      NoNameOrNumber,
      AccessDenied,
      HardwareFailure,
      GeneralFailure,
      ConnectionLost,
      NotInstalled,
      NumStatuses
    };
    Status GetStatus() const;
    /* Get the current status of the RAS connection.

       <H2>Returns:</H2>
       Status code.
     */

    PString GetAddress();
    /* Get the IP address in dotted decimal form for the RAS connection.

       <H2>Returns:</H2>
       String for IP address, or empty string if none.
     */

    DWORD GetErrorCode() const { return osError; }
    /* Get the error code for the last operation.

       <H2>Returns:</H2>
       Operating system error code.
     */


    static PStringArray GetAvailableNames();
    /* Get an array of names for all of the available remote connections on
       this system.

       <H2>Returns:</H2>
       Array of strings for remote connection names.
     */

    struct Configuration {
      PString device;
      PString phoneNumber;
      PString ipAddress;
      PString dnsAddress;
      PString script;
      PINDEX  subEntries;
      BOOL    dialAllSubEntries;
    };

    Status GetConfiguration(
      Configuration & config  // Configuration of remote connection
    );
    static Status GetConfiguration(
      const PString & name,   // Remote connection name to get configuration
      Configuration & config  // Configuration of remote connection
    );
    /* Get the configuration of the specified remote access connection.

       <H2>Returns:</H2>
       <CODE>Connected</CODE> if the configuration information was obtained,
       <CODE>NoNameOrNumber</CODE> if the particular RAS name does not exist,
       <CODE>NotInstalled</CODE> if there is no RAS support in the operating system,
       <CODE>GeneralFailure</CODE> on any other error.
     */

    Status SetConfiguration(
      const Configuration & config,  // Configuration of remote connection
      BOOL create = FALSE            // Flag to create connection if not present
    );
    static Status SetConfiguration(
      const PString & name,          // Remote connection name to configure
      const Configuration & config,  // Configuration of remote connection
      BOOL create = FALSE            // Flag to create connection if not present
    );
    /* Set the configuration of the specified remote access connection.

       <H2>Returns:</H2>
       <CODE>Connected</CODE> if the configuration information was set,
       <CODE>NoNameOrNumber</CODE> if the particular RAS name does not exist,
       <CODE>NotInstalled</CODE> if there is no RAS support in the operating system,
       <CODE>GeneralFailure</CODE> on any other error.
     */

    static Status RemoveConfiguration(
      const PString & name          // Remote connection name to configure
    );
    /* Remove the specified remote access connection.

       <H2>Returns:</H2>
       <CODE>Connected</CODE> if the configuration information was removed,
       <CODE>NoNameOrNumber</CODE> if the particular RAS name does not exist,
       <CODE>NotInstalled</CODE> if there is no RAS support in the operating system,
       <CODE>GeneralFailure</CODE> on any other error.
     */

    
  protected:
    PString remoteName;
    PString userName;
    PString password;
    DWORD osError;

  private:
    PRemoteConnection(const PRemoteConnection &) { }
    void operator=(const PRemoteConnection &) { }
    void Construct();


// Class declaration continued in platform specific header file ///////////////
