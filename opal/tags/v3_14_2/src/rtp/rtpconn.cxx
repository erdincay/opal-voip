/*
 * rtpconn.cxx
 *
 * Connection abstraction
 *
 * Open Phone Abstraction Library (OPAL)
 *
 * Copyright (C) 2007 Post Increment
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
 * The Original Code is Open Phone Abstraction Library.
 *
 * The Initial Developer of the Original Code is Post Increment
 *
 * Contributor(s): ______________________________________.
 *
 * $Revision$
 * $Author$
 * $Date$
 */

#include <ptlib.h>

#ifdef P_USE_PRAGMA
#pragma implementation "rtpconn.h"
#endif

#include <opal_config.h>

#include <rtp/rtpconn.h>
#include <rtp/rtpep.h>
#include <opal/manager.h>
#include <codec/rfc2833.h>
#include <t38/t38proto.h>
#include <opal/patch.h>

#if OPAL_VIDEO
#include <codec/vidcodec.h>
#endif

#include <algorithm>


#define new PNEW


///////////////////////////////////////////////////////////////////////////////

OpalRTPConnection::OpalRTPConnection(OpalCall & call,
                             OpalRTPEndPoint  & ep,
                                const PString & token,
                                   unsigned int options,
                                StringOptions * stringOptions)
  : OpalConnection(call, ep, token, options, stringOptions)
  , m_remoteBehindNAT(false)
{
  m_rfc2833Handler = new OpalRFC2833Proto(PCREATE_NOTIFIER(OnUserInputInlineRFC2833), OpalRFC2833);
  PTRACE_CONTEXT_ID_TO(m_rfc2833Handler);

#if OPAL_T38_CAPABILITY
  m_ciscoNSEHandler = new OpalRFC2833Proto(PCREATE_NOTIFIER(OnUserInputInlineRFC2833), OpalCiscoNSE);
  PTRACE_CONTEXT_ID_TO(m_ciscoNSEHandler);
#endif
}

OpalRTPConnection::~OpalRTPConnection()
{
  delete m_rfc2833Handler;
#if OPAL_T38_CAPABILITY
  delete m_ciscoNSEHandler;
#endif
}


void OpalRTPConnection::OnReleased()
{
  OpalConnection::OnReleased();

  for (SessionMap::iterator it = m_sessions.begin(); it != m_sessions.end(); ++it) {
    OpalRTPSession * rtp = dynamic_cast<OpalRTPSession * >(&*it->second);
    if (rtp != NULL && (rtp->GetPacketsSent() != 0 || rtp->GetPacketsReceived() != 0))
      rtp->SendBYE();
  }

  if (m_rfc2833Handler != NULL)
    m_rfc2833Handler->UseRTPSession(false, NULL);
#if OPAL_T38_CAPABILITY
  if (m_ciscoNSEHandler != NULL)
    m_ciscoNSEHandler->UseRTPSession(false, NULL);
#endif

  m_sessions.RemoveAll();
}


unsigned OpalRTPConnection::GetNextSessionID(const OpalMediaType & mediaType, bool isSource)
{
  unsigned nextSessionId = 1;

  for (SessionMap::iterator it = m_sessions.begin(); it != m_sessions.end(); ++it) {
    if (nextSessionId <= it->first)
      nextSessionId = it->first+1;
  }

  if (GetMediaStream(mediaType, isSource) != NULL)
    return nextSessionId;

  OpalMediaStreamPtr mediaStream = GetMediaStream(mediaType, !isSource);
  if (mediaStream != NULL)
    return mediaStream->GetSessionID();

  unsigned defaultSessionId = mediaType->GetDefaultSessionId();
  if (defaultSessionId >= nextSessionId ||
      GetMediaStream(defaultSessionId,  isSource) != NULL ||
      GetMediaStream(defaultSessionId, !isSource) != NULL)
    return nextSessionId;

  return defaultSessionId;
}


vector<bool> OpalRTPConnection::CreateAllMediaSessions(CreateMediaSessionsSecurity security)
{
  OpalMediaTypeList allMediaTypes = m_localMediaFormats.GetMediaTypes();
  allMediaTypes.PrioritiseAudioVideo();

  const PStringArray cryptoSuites = GetMediaCryptoSuites();

  vector<bool> openedMediaSessions(allMediaTypes.size()*cryptoSuites.GetSize()+1);

  for (OpalMediaTypeList::iterator iterMediaType = allMediaTypes.begin(); iterMediaType != allMediaTypes.end(); ++iterMediaType) {
    const OpalMediaType & mediaType = *iterMediaType;
    if (!PAssert(mediaType.GetDefinition() != NULL, PLogicError))
      continue;

    if (GetAutoStart(mediaType) == OpalMediaType::DontOffer) {
      PTRACE(4, "RTPCon\tNot offerring " << mediaType);
      continue;
    }

    // See if any media formats of this session id, so don't create unused RTP session
    if (!m_localMediaFormats.HasType(mediaType)) {
      PTRACE(3, "RTPCon\tNo media formats of type " << mediaType << ", not creating media session");
      continue;
    }

    for (PINDEX csIdx = 0; csIdx < cryptoSuites.GetSize(); ++csIdx) {
      PCaselessString cryptoSuiteName = cryptoSuites[csIdx];
      if (cryptoSuiteName == OpalMediaCryptoSuite::ClearText() && !(security & e_ClearMediaSession)) {
        PTRACE(4, "RTPCon\tSkipping " << cryptoSuiteName << " as secure media required.");
        continue;
      }
      if (cryptoSuiteName != OpalMediaCryptoSuite::ClearText() && !(security & e_SecureMediaSession)) {
        PTRACE(4, "RTPCon\tSkipping " << cryptoSuiteName << " as non-secure media required.");
        continue;
      }

      OpalMediaCryptoSuite * cryptoSuite = OpalMediaCryptoSuiteFactory::CreateInstance(cryptoSuiteName);
      if (cryptoSuite == NULL) {
        PTRACE(1, "RTPCon\tCannot create crypto suite for " << cryptoSuiteName);
        continue;
      }

      PCaselessString sessionType = mediaType->GetMediaSessionType();
      if (!cryptoSuite->ChangeSessionType(sessionType)) {
        PTRACE(3, "RTPCon\tCannot use crypto suite " << cryptoSuiteName << " with media type " << mediaType);
        continue;
      }

      OpalMediaSession * session = NULL;

      unsigned nextSessionId = 1;
      for (SessionMap::const_iterator it = m_sessions.begin(); it != m_sessions.end(); ++it) {
        if (it->second->GetMediaType() == mediaType && it->second->GetSessionType() == sessionType) {
          session = it->second;
          break;
        }
        if (nextSessionId <= it->first)
          nextSessionId = it->first+1;
      }

      if (session == NULL && (session = UseMediaSession(nextSessionId, mediaType, sessionType)) == NULL) {
        PTRACE(1, "RTPCon\tCould not create RTP session for media type " << mediaType);
        continue;
      }

      session->OfferCryptoSuite(cryptoSuiteName);

      openedMediaSessions[session->GetSessionID()] = true;
    }
  }

  return openedMediaSessions;
}


OpalMediaSession * OpalRTPConnection::GetMediaSession(unsigned sessionID) const
{
  SessionMap::const_iterator it = m_sessions.find(sessionID);
  return it != m_sessions.end() ? it->second : NULL;
}


OpalMediaSession * OpalRTPConnection::FindSessionByMediaType(const OpalMediaType & mediaType) const
{
  for (SessionMap::const_iterator it = m_sessions.begin(); it != m_sessions.end(); ++it) {
    if (it->second->GetMediaType() == mediaType)
      return &*it->second;
  }
  return NULL;
}


OpalMediaSession * OpalRTPConnection::FindSessionByLocalPort(WORD port) const
{
  for (SessionMap::const_iterator it = m_sessions.begin(); it != m_sessions.end(); ++it) {
    OpalRTPSession * rtp = dynamic_cast<OpalRTPSession * >(&*it->second);
    if (rtp != NULL && (rtp->GetLocalDataPort() == port || rtp->GetLocalControlPort() == port))
      return rtp;
  }

  return NULL;
}


OpalMediaSession * OpalRTPConnection::UseMediaSession(unsigned sessionId,
                                                      const OpalMediaType & mediaType,
                                                      const PString & sessionType)
{
  SessionMap::iterator it = m_sessions.find(sessionId);
  if (it != m_sessions.end()) {
    PTRACE(4, "RTPCon\tFound existing " << it->second->GetMediaType() << " session " << sessionId);
    return it->second;
  }

  OpalMediaSession * session = CreateMediaSession(sessionId, mediaType, sessionType);
  if (session != NULL)
    m_sessions.SetAt(sessionId, session);

  return session;
}


OpalMediaSession * OpalRTPConnection::CreateMediaSession(unsigned sessionId,
                                                         const OpalMediaType & mediaType,
                                                         const PString & sessionType)
{
  OpalMediaSession::Init init(*this, sessionId, mediaType, m_remoteBehindNAT);

  {
    OpalTransportAddressArray transports;
    PSafePtr<OpalConnection> otherConnection = GetOtherPartyConnection();
    if (otherConnection != NULL && otherConnection->GetMediaTransportAddresses(*this, mediaType, transports)) {
      // Make sure we do not include any transcoded format combinations
      m_localMediaFormats.Remove(PString('@')+mediaType);
      m_localMediaFormats += otherConnection->GetMediaFormats();
      PTRACE(4, "RTPCon\tCreated dummy " << mediaType << " session " << sessionId << " using formats " << setfill(',') << m_localMediaFormats);
      return new OpalDummySession(init, transports);
    }
  }

  PString actualSessionType = sessionType;
  if (actualSessionType.IsEmpty())
    actualSessionType = mediaType->GetMediaSessionType();

  OpalMediaSession * session = OpalMediaSessionFactory::CreateInstance(actualSessionType, init);
  if (session == NULL) {
    PTRACE(1, "RTPCon\tCannot create session for " << actualSessionType);
    return NULL;
  }

  PTRACE(4, "RTPCon\tCreated " << mediaType << " session " << sessionId << " using " << actualSessionType);
  return session;
}


bool OpalRTPConnection::GetMediaTransportAddresses(OpalConnection & otherConnection,
                                              const OpalMediaType & mediaType,
                                        OpalTransportAddressArray & transports) const
{
  if (NoMediaBypass(otherConnection, mediaType))
    return false;

  for (SessionMap::const_iterator session = m_sessions.begin(); session != m_sessions.end(); ++session) {
    if (session->second->GetMediaType() == mediaType &&
        transports.SetAddressPair(session->second->GetRemoteAddress(true), session->second->GetRemoteAddress(false))) {
      PTRACE(3, "RTPCon\tGetMediaTransportAddresses of " << mediaType << " found session addresses "
              << setfill(',') << transports << " for " << otherConnection << " on " << *this);
      return true;
    }
  }

  return OpalConnection::GetMediaTransportAddresses(otherConnection, mediaType, transports);
}


bool OpalRTPConnection::NoMediaBypass(const OpalConnection & otherConnection, const OpalMediaType & mediaType) const
{
  if (endpoint.GetManager().GetMediaTransferMode(*this, otherConnection, mediaType) == OpalManager::MediaTransferBypass)
    return false;

  PTRACE(4, "OpalCon\tGetMediaTransportAddresses for " << mediaType << " not using MediaTransferBypass.");
  return true;
}


#if OPAL_RTP_FEC
OpalMediaFormatList OpalRTPConnection::NegotiateFECMediaFormats(OpalMediaSession & session)
{
  OpalMediaFormatList fecFormats;

  OpalRTPSession * rtpSession = dynamic_cast<OpalRTPSession *>(&session);
  if (rtpSession != NULL) {
    OpalMediaFormatList remoteFormats = GetMediaFormats();

    for (OpalMediaFormatList::iterator it = m_localMediaFormats.begin(); it != m_localMediaFormats.end(); ++it) {
      if (it->GetMediaType() == OpalFEC::MediaType() && it->GetOptionString(OpalFEC::MediaTypeOption()) == session.GetMediaType()) {
        OpalMediaFormatList::const_iterator fmt = remoteFormats.FindFormat(*it);
        if (fmt != remoteFormats.end()) {
          fecFormats += *fmt;

          if (fmt->GetName().NumCompare(OPAL_REDUNDANT_PREFIX) == EqualTo)
            rtpSession->SetRedundencyPayloadType(fmt->GetPayloadType());
          else if (fmt->GetName().NumCompare(OPAL_ULP_FEC_PREFIX) == EqualTo)
            rtpSession->SetUlpFecPayloadType(fmt->GetPayloadType());

          PTRACE(4, "OpalCon\tAccepted FEC format " << *fmt << ", pt=" << fmt->GetPayloadType());
        }
      }
    }
  }

  return fecFormats;
}
#endif // OPAL_RTP_FEC


bool OpalRTPConnection::ChangeSessionID(unsigned fromSessionID, unsigned toSessionID)
{
  SessionMap::iterator from = m_sessions.find(fromSessionID);
  if (from == m_sessions.end()) {
    PTRACE(2, "RTPCon\tAttempt to renumber unknown session " << fromSessionID << " to session ID " << toSessionID);
    return false;
  }

  SessionMap::iterator to = m_sessions.find(toSessionID);
  if (to != m_sessions.end()) {
    PTRACE(2, "RTPCon\tAttempt to renumber session " << fromSessionID << " to existing session ID " << toSessionID);

    m_sessions.erase(from);
  }
  else {
    PTRACE(3, "RTPCon\tChanging session ID " << fromSessionID << " to " << toSessionID);

    PSafePtr<OpalMediaSession> session = from->second;
    m_sessions.DisallowDeleteObjects();
    m_sessions.erase(from);
    m_sessions.AllowDeleteObjects();
    m_sessions.SetAt(toSessionID, session);
  }

  for (OpalMediaStreamPtr stream(mediaStreams, PSafeReference); stream != NULL; ++stream) {
    if (stream->GetSessionID() == fromSessionID) {
      stream->SetSessionID(toSessionID);
      OpalMediaPatchPtr patch = stream->GetPatch();
      if (patch != NULL) {
        patch->GetSource().SetSessionID(toSessionID);
        OpalMediaStreamPtr otherStream;
        for (PINDEX i = 0; (otherStream = patch->GetSink(i)) != NULL; ++i)
          otherStream->SetSessionID(toSessionID);
      }
    }
  }

  return true;
}


void OpalRTPConnection::ReplaceMediaSession(unsigned sessionId, OpalMediaSession * mediaSession)
{
  {
    SessionMap::iterator it = m_sessions.find(sessionId);
    if (it == m_sessions.end()) {
      PTRACE(4, "RTPCon\tReplacing empty session " << sessionId);
      m_sessions.SetAt(sessionId, mediaSession);
      return;
    }

    OpalTransportAddress remoteMedia = it->second->GetRemoteAddress(true);
    OpalTransportAddress remoteCtrl = it->second->GetRemoteAddress(false);

    OpalMediaSession::Transport transport = it->second->DetachTransport();
    mediaSession->AttachTransport(transport);

    mediaSession->SetRemoteAddress(remoteMedia, true);
    mediaSession->SetRemoteAddress(remoteCtrl, false);
    PTRACE(4, "RTPCon\tReplacing session " << sessionId << " media=" << remoteMedia << " ctrl=" << remoteCtrl);
  }

  OpalRTPSession * rtpSession = dynamic_cast<OpalRTPSession *>(mediaSession);
  if (rtpSession != NULL && rtpSession->IsAudio()) {
    if (m_rfc2833Handler != NULL)
      m_rfc2833Handler->UseRTPSession(false, rtpSession);

#if OPAL_T38_CAPABILITY
    if (m_ciscoNSEHandler != NULL)
      m_ciscoNSEHandler->UseRTPSession(false, rtpSession);
#endif
  }

  m_sessions.SetAt(sessionId, mediaSession);
}


void OpalRTPConnection::ReleaseMediaSession(unsigned sessionID)
{
  SessionMap::iterator it = m_sessions.find(sessionID);
  if (it == m_sessions.end()) {
    PTRACE(2, "RTPCon\tAttempt to release unknown session " << sessionID);
    return;
  }

  OpalRTPSession * rtpSession = dynamic_cast<OpalRTPSession *>(&*it->second);
  if (rtpSession != NULL && rtpSession->IsAudio()) {
    if (m_rfc2833Handler != NULL)
      m_rfc2833Handler->UseRTPSession(false, NULL);

#if OPAL_T38_CAPABILITY
    if (m_ciscoNSEHandler != NULL)
      m_ciscoNSEHandler->UseRTPSession(false, NULL);
#endif
  }

  m_sessions.erase(it);
}


bool OpalRTPConnection::SetSessionQoS(OpalRTPSession * /*session*/)
{
  return true;
}


void OpalRTPConnection::DetermineRTPNAT(const OpalTransport & transport, const OpalTransportAddress & signalAddr)
{
  PIPSocket::Address localAddr, peerAddr, sigAddr;

  transport.GetLocalAddress().GetIpAddress(localAddr);
  transport.GetRemoteAddress().GetIpAddress(peerAddr);
  signalAddr.GetIpAddress(sigAddr);

  if (dynamic_cast<OpalRTPEndPoint &>(endpoint).IsRTPNATEnabled(*this, localAddr, peerAddr, sigAddr, !IsOriginating()))
    m_remoteBehindNAT = true;
}


bool OpalRTPConnection::OnMediaCommand(OpalMediaStream & stream, const OpalMediaCommand & command)
{
  PTRACE(5, "RTPCon\tOnMediaCommand \"" << command << "\" for " << *this);

  if (stream.IsSource() != (&stream.GetConnection() == this))
    return OpalConnection::OnMediaCommand(stream, command);

  unsigned sessionID = stream.GetSessionID();
  OpalRTPSession * session = dynamic_cast<OpalRTPSession *>(GetMediaSession(sessionID));
  if (session == NULL)
    return OpalConnection::OnMediaCommand(stream, command);

#if OPAL_VIDEO

  OpalVideoFormat::RTCPFeedback rtcp_fb = stream.GetMediaFormat().GetOptionEnum(OpalVideoFormat::RTCPFeedbackOption(),
                                                                                OpalVideoFormat::e_NoRTCPFb);
  if (rtcp_fb == OpalVideoFormat::e_NoRTCPFb) {
    OpalMediaStreamPtr source = GetMediaStream(sessionID, true);
    if (source != NULL)
      rtcp_fb = source->GetMediaFormat().GetOptionEnum(OpalVideoFormat::RTCPFeedbackOption(), OpalVideoFormat::e_NoRTCPFb);
  }

  const OpalMediaFlowControl * flow = dynamic_cast<const OpalMediaFlowControl *>(&command);
  if (flow != NULL) {
    if (rtcp_fb & OpalVideoFormat::e_TMMBR) {
      session->SendFlowControl(flow->GetMaxBitRate());
      return true;
    }
    PTRACE(3, "RTPCon\tRemote not capable of flow control (TMMBR)");
    return OpalConnection::OnMediaCommand(stream, command);
  }

  const OpalTemporalSpatialTradeOff * tsto = dynamic_cast<const OpalTemporalSpatialTradeOff *>(&command);
  if (tsto != NULL) {
    if (rtcp_fb & OpalVideoFormat::e_TSTR) {
      session->SendTemporalSpatialTradeOff(tsto->GetTradeOff());
      return true;
    }
    PTRACE(3, "RTPCon\tRemote not capable of Temporal/Spatial Tradeoff (TSTR)");
    return OpalConnection::OnMediaCommand(stream, command);
  }

  if (PIsDescendant(&command, OpalVideoUpdatePicture)) {
    unsigned mask = m_stringOptions.GetInteger(OPAL_OPT_VIDUP_METHODS, OPAL_OPT_VIDUP_METHOD_DEFAULT);
    if ((mask&(OPAL_OPT_VIDUP_METHOD_RTCP | OPAL_OPT_VIDUP_METHOD_PLI | OPAL_OPT_VIDUP_METHOD_FIR)) != 0) {
      bool has_AVPF_PLI = (rtcp_fb & OpalVideoFormat::e_PLI) || (mask & OPAL_OPT_VIDUP_METHOD_PLI);
      bool has_AVPF_FIR = (rtcp_fb & OpalVideoFormat::e_FIR) || (mask & OPAL_OPT_VIDUP_METHOD_FIR);
      PTRACE(4, "RTPCon\tPLI=" << ((rtcp_fb & OpalVideoFormat::e_PLI) ? "allowed" : ((mask & OPAL_OPT_VIDUP_METHOD_PLI) ? "forced" : "disabled"))
                    << " FIR=" << ((rtcp_fb & OpalVideoFormat::e_FIR) ? "allowed" : ((mask & OPAL_OPT_VIDUP_METHOD_FIR) ? "forced" : "disabled")));

      if (has_AVPF_PLI && has_AVPF_FIR)
        session->SendIntraFrameRequest(false, PIsDescendant(&command, OpalVideoPictureLoss));
      else if (has_AVPF_PLI)
        session->SendIntraFrameRequest(false, true);  // More common, use RFC4585 PLI
      else if (has_AVPF_FIR)
        session->SendIntraFrameRequest(false, false); // Unusual, but possible, use RFC5104 FIR
      else
        session->SendIntraFrameRequest(true, false);  // Fall back to RFC2032

#if OPAL_STATISTICS
      m_VideoUpdateRequestsSent++;
#endif

      return true;
    }

    PTRACE(4, "RTPCon\tRTCP Intra-Frame Request disabled in string options");
  }
#endif // OPAL_VIDEO

  return OpalConnection::OnMediaCommand(stream, command);
}


PBoolean OpalRTPConnection::SendUserInputTone(char tone, unsigned duration)
{
  if (GetRealSendUserInputMode() == SendUserInputAsRFC2833) {
    if (
#if OPAL_T38_CAPABILITY
        m_ciscoNSEHandler->SendToneAsync(tone, duration) ||
#endif
         m_rfc2833Handler->SendToneAsync(tone, duration))
      return true;

    PTRACE(2, "RTPCon\tCould not send tone '" << tone << "' via RFC2833.");
  }

  return OpalConnection::SendUserInputTone(tone, duration);
}


OpalMediaStream * OpalRTPConnection::CreateMediaStream(const OpalMediaFormat & mediaFormat,
                                                       unsigned sessionID,
                                                       PBoolean isSource)
{
  OpalMediaSession * mediaSession = UseMediaSession(sessionID, mediaFormat.GetMediaType());
  if (mediaSession != NULL)
    return mediaSession->CreateMediaStream(mediaFormat, sessionID, isSource);

  PTRACE(1, "RTPCon\tUnable to create media stream for session " << sessionID);
  return NULL;
}


void OpalRTPConnection::AdjustMediaFormats(bool   local,
                           const OpalConnection * otherConnection,
                            OpalMediaFormatList & mediaFormats) const
{
  if (otherConnection == NULL && local) {
    OpalMediaFormatList::iterator fmt = mediaFormats.begin();
    while (fmt != mediaFormats.end()) {
      if (fmt->IsTransportable())
        ++fmt;
      else
        mediaFormats -= *fmt++;
    }
#if OPAL_RTP_FEC
    mediaFormats += OpalRedundantAudio;
    mediaFormats += OpalUlpFecAudio;
 #if OPAL_VIDEO
    mediaFormats += OpalRedundantVideo;
    mediaFormats += OpalUlpFecVideo;
 #endif
#endif
  }

  OpalConnection::AdjustMediaFormats(local, otherConnection, mediaFormats);
}


void OpalRTPConnection::OnPatchMediaStream(PBoolean isSource, OpalMediaPatch & patch)
{
  OpalConnection::OnPatchMediaStream(isSource, patch);

  if (patch.GetSource().GetMediaFormat().GetMediaType() != OpalMediaType::Audio())
    return;

  OpalRTPSession * rtpSession = dynamic_cast<OpalRTPSession *>(GetMediaSession(patch.GetSource().GetSessionID()));
  if (rtpSession == NULL)
    return;

  if (m_rfc2833Handler != NULL)
    m_rfc2833Handler->UseRTPSession(isSource, rtpSession);

#if OPAL_T38_CAPABILITY
  if (m_ciscoNSEHandler != NULL)
    m_ciscoNSEHandler->UseRTPSession(isSource, rtpSession);
#endif
}

void OpalRTPConnection::OnUserInputInlineRFC2833(OpalRFC2833Info & info, P_INT_PTR)
{
  GetEndPoint().GetManager().QueueDecoupledEvent(
          new PSafeWorkArg2<OpalConnection, char, unsigned>(
                 this, info.GetTone(),
                 info.GetDuration(),
                 &OpalConnection::OnUserInputTone));
}


bool OpalRTPConnection::GarbageCollection()
{
  return m_sessions.DeleteObjectsToBeRemoved() & OpalConnection::GarbageCollection();
}


void OpalRTPConnection::SessionMap::MoveFrom(SessionMap & other)
{
  *this = other;
  AllowDeleteObjects(); // We now own the objects, need to put this back on

  // Remove from other without deleting them
  bool del = other.deleteObjects;
  other.DisallowDeleteObjects();
  other.RemoveAll();
  other.AllowDeleteObjects(del);
}


/////////////////////////////////////////////////////////////////////////////
