/*
 * SpanDSP - a series of DSP components for telephony
 *
 * adsi.h - Analogue display services interface and other call ID related handling.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2003 Steve Underwood
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: adsi.h,v 1.15 2006/10/24 13:22:01 steveu Exp $
 */

/*! \file */

#if !defined(_ADSI_H_)
#define _ADSI_H_

/*! \page adsi_page ADSI transmission and reception
\section adsi_page_sec_1 What does it do?
Although ADSI has a specific meaning in some places, the term is used here to indicate
any form of Analogue Display Service Interface, which includes caller ID, SMS, and others.

The ADSI module provides for the transmission and reception of ADSI messages
in various formats. Currently, the supported formats are:
    - CLASS (Custom Local Area Signaling Services) standard, published by Bellcore:

    - ACLIP (Analog Calling Line Identity Presentation) standard, published by the
      Telecommunications Authority of Singapore:

    - CLIP (Calling Line Identity Presentation) standard, published by ETSI.

    - JCLIP (Japanese Calling Line Identity Presentation) standard, published by NTT.
        
    - TDD (Telecommunications Device for the Deaf).

\section adsi_page_sec_2 How does it work?

\section adsi_page_sec_2a The Bellcore CLASS specification
Most FSK based CLI formats are similar to the US CLASS one, which is as follows:

The alert tone for CLI during a call is at least 100ms of silence, then
2130Hz + 2750Hz for 88ms to 110ms. When CLI is presented at ringing time,
this tone is not sent. In the US, CLI is usually sent between the first
two rings. This silence period is long in the US, so the message fits easily.
In other places, where the standard ring tone has much smaller silences,
a line voltage reversal is used to wake up a power saving receiver, then the
message is sent, then the phone begins to ring.
    
The message is sent using a Bell 202 FSK modem. The data rate is 1200 bits
per second. The message protocol uses 8-bit data words (bytes), each bounded
by a start bit and a stop bit.

Channel     Carrier     Message     Message     Data        Checksum
Seizure     Signal      Type        Length      Word(s)     Word
Signal                  Word        Word
    
\section adsi_page_sec_2a1 CHANNEL SEIZURE SIGNAL
The channel seizure is 30 continuous bytes of 55h (01010101), including
the start and stop bits (i.e. 300 bits of alternations in total).
This provides a detectable alternating function to the CPE (i.e. the
modem data pump).
    
\section adsi_page_sec_2a2 CARRIER SIGNAL
The carrier signal consists of 180 bits of 1s. This may be reduced to 80
bits of 1s for caller ID on call waiting.
    
\section adsi_page_sec_2a3 MESSAGE TYPE WORD
Various message types are defined. The common ones for the US CLASS 
standard are:
    - Type 0x04 (SDMF) - single data message. Simple caller ID (CND)
    - Type 0x80 (MDMF) - multiple data message. A more flexible caller ID,
                         with extra information.
    
\section adsi_page_sec_2a4 MESSAGE LENGTH WORD
The message length word specifies the total number of data words
to follow.
    
\section adsi_page_sec_2a5 DATA WORDS
The data words contain the actual message.
    
\section adsi_page_sec_2a6 CHECKSUM WORD
The Checksum Word contains the twos complement of the modulo 256
sum of the other words in the data message (i.e., message type,
message length, and data words).  The receiving equipment may
calculate the modulo 256 sum of the received words and add this
sum to the received checksum word.  A result of zero generally
indicates that the message was correctly received.  Message
retransmission is not supported. The sumcheck word should be followed
by a minimum of two stop bits.

\section adsi_page_sec_2b The ETSI CLIP specification
The ETSI CLIP specification uses similar messages to the Bellcore specification.
They are not, however, identical. First, ETSI use the V.23 modem standard, rather
than Bell 202. Second, different fields, and different message types are available.

\section adsi_page_sec_2e The ETSI caller ID by DTMF specification
CLI by DTMF is usually sent in a very simple way. The exchange does not give
any prior warning (no reversal, or ring) to wake up the receiver. It just
sends one of the following DTMF strings:
    
    - A<phone number>#
    - D1#     Number not available because the caller has restricted it.
    - D2#     Number not available because the call is international.
    - D3#     Number not available due to technical reasons.
*/

enum
{
    ADSI_STANDARD_NONE = 0,
    ADSI_STANDARD_CLASS = 1,
    ADSI_STANDARD_CLIP = 2,
    ADSI_STANDARD_ACLIP = 3,
    ADSI_STANDARD_JCLIP = 4,
    ADSI_STANDARD_CLIP_DTMF = 5,
    ADSI_STANDARD_TDD = 6
};

/* In some of the messages code characters are used, as follows:
        'C' for public callbox
        'L' for long distance
        'O' for overseas
        'P' for private
        'S' for service conflict */

/*! Definitions for generic caller ID message type IDs */
/*! Complete caller ID message */
#define CLIDINFO_CMPLT          0x100
/*! Date, time, phone #, name */
#define CLIDINFO_GENERAL        0x101
/*! Caller ID */
#define CLIDINFO_CALLID         0x102
/*! See frame type equates */
#define CLIDINFO_FRAMETYPE      0x103

/*! Definitions for CLASS (Custom Local Area Signaling Services) */
/*! Single data message caller ID */
#define CLASS_SDMF_CALLERID     0x04
/*! Multiple data message caller ID */
#define CLASS_MDMF_CALLERID     0x80
/*! Single data message message waiting */
#define CLASS_SDMF_MSG_WAITING  0x06
/*! Multiple data message message waiting */
#define CLASS_MDMF_MSG_WAITING  0x82

/*! CLASS MDMF message IDs */
/*! Date and time (MMDDHHMM) */
#define MCLASS_DATETIME         0x01
/*! Caller number */
#define MCLASS_CALLER_NUMBER    0x02
/*! Dialed number */
#define MCLASS_DIALED_NUMBER    0x03
/*! Caller number absent: 'O' or 'P' */
#define MCLASS_ABSENCE1         0x04
/*! Call forward: universal ('0'), on busy ('1'), or on unanswered ('2') */
#define MCLASS_REDIRECT         0x05
/*! Long distance: 'L' */
#define MCLASS_QUALIFIER        0x06
/*! Caller's name */
#define MCLASS_CALLER_NAME      0x07
/*! Caller's name absent: 'O' or 'P' */
#define MCLASS_ABSENCE2         0x08

/*! CLASS MDMF message waiting message IDs */
/*! Message waiting/not waiting */
#define MCLASS_VISUAL_INDICATOR 0x0B

/*! Definitions for CLIP (Calling Line Identity Presentation) */
/*! Multiple data message caller ID */
#define CLIP_MDMF_CALLERID      0x80
/*! Multiple data message message waiting */
#define CLIP_MDMF_MSG_WAITING   0x82
/*! Multiple data message charge information */
#define CLIP_MDMF_CHARGE_INFO   0x86
/*! Multiple data message SMS */
#define CLIP_MDMF_SMS           0x89

/*! CLIP message IDs */
/*! Date and time (MMDDHHMM) */
#define CLIP_DATETIME           0x01
/*! Caller number */
#define CLIP_CALLER_NUMBER      0x02
/*! Dialed number */
#define CLIP_DIALED_NUMBER      0x03
/*! Caller number absent: 'O' or 'P' */
#define CLIP_ABSENCE1           0x04
/*! Caller's name */
#define CLIP_CALLER_NAME        0x07
/*! Caller's name absent: 'O' or 'P' */
#define CLIP_ABSENCE2           0x08
/*! Visual indicator */
#define CLIP_VISUAL_INDICATOR   0x0B
/*! Message ID */
#define CLIP_MESSAGE_ID         0x0D
/*! Voice call, ring-back-when-free call, or msg waiting call */
#define CLIP_CALLTYPE           0x11
/*! Number of messages */
#define CLIP_NUM_MSG            0x13
/*! Redirecting number */
#define CLIP_REDIR_NUMBER       0x03
/*! Charge */
#define CLIP_CHARGE             0x20
/*! Duration of the call */
#define CLIP_DURATION           0x23
/*! Additional charge */
#define CLIP_ADD_CHARGE         0x21
/*! Display information */
#define CLIP_DISPLAY_INFO       0x50
/*! Service information */
#define CLIP_SERVICE_INFO       0x55

/*! Definitions for A-CLIP (Analog Calling Line Identity Presentation) */
/*! Single data message caller ID frame   */
#define ACLIP_SDMF_CALLERID     0x04
/*! Multiple data message caller ID frame */
#define ACLIP_MDMF_CALLERID     0x80

/*! A-CLIP MDM message IDs */
/*! Date and time (MMDDHHMM) */
#define ACLIP_DATETIME          0x01
/*! Caller number */
#define ACLIP_CALLER_NUMBER     0x02
/*! Dialed number */
#define ACLIP_DIALED_NUMBER     0x03
/*! Caller number absent: 'O' or 'P' */
#define ACLIP_ABSENCE1          0x04
/*! Call forward: universal, on busy, or on unanswered */
#define ACLIP_REDIRECT          0x05
/*! Long distance call: 'L' */
#define ACLIP_QUALIFIER         0x06
/*! Caller's name */
#define ACLIP_CALLER_NAME       0x07
/*! Caller's name absent: 'O' or 'P' */
#define ACLIP_ABSENCE2          0x08

/*! Definitions for J-CLIP (Japan Calling Line Identity Presentation) */
/*! Multiple data message caller ID frame */
#define JCLIP_MDMF_CALLERID     0x40

/*! J-CLIP MDM message IDs */
/*! Caller number */
#define JCLIP_CALLER_NUMBER     0x02
/*! Caller number data extension signal */
#define JCLIP_CALLER_NUM_DES    0x21
/*! Dialed number */
#define JCLIP_DIALED_NUMBER     0x09
/*! Dialed number data extension signal */
#define JCLIP_DIALED_NUM_DES    0x22
/*! Caller number absent: 'C', 'O', 'P' or 'S' */
#define JCLIP_ABSENCE           0x04

/*! Definitions for CLIP-DTMF */
/*! Caller number */
#define CLIP_DTMF_CALLER_NUMBER 'A'
/*! Caller number absent: private (1), overseas (2) or not available (3) */
#define CLIP_DTMF_ABSENCE1      'D'

/*!
    ADSI transmitter descriptor. This contains all the state information for an ADSI
    (caller ID, CLASS, CLIP, ACLIP) transmit channel.
 */
typedef struct
{
    int standard;

    tone_gen_descriptor_t alert_tone_desc;
    tone_gen_state_t alert_tone_gen;
    fsk_tx_state_t fsktx;
    dtmf_tx_state_t dtmftx;
    async_tx_state_t asynctx;
    
    int tx_signal_on;
    
    int byteno;
    int bitpos;
    int bitno;
    uint8_t msg[256];
    int msg_len;
    int ones_len;
    int baudot_shift;
    
    logging_state_t logging;
} adsi_tx_state_t;

/*!
    ADSI receiver descriptor. This contains all the state information for an ADSI
    (caller ID, CLASS, CLIP, ACLIP) receive channel.
 */
typedef struct
{
    put_msg_func_t put_msg;
    void *user_data;
    int standard;

    fsk_rx_state_t fskrx;
    dtmf_rx_state_t dtmfrx;
    async_rx_state_t asyncrx;
    
    int consecutive_ones;
    int bitpos;
    int in_progress;
    uint8_t msg[256];
    int msg_len;
    int baudot_shift;
    
    /*! A count of the framing errors. */
    int framing_errors;

    logging_state_t logging;
} adsi_rx_state_t;

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Initialise an ADSI receive context.
    \param s The ADSI receive context.
    \param standard The code for the ADSI standard to be used.
    \param put_msg A callback routine called to deliver the received messages
           to the application.
    \param user_data An opaque pointer for the callback routine.
*/
void adsi_rx_init(adsi_rx_state_t *s, int standard, put_msg_func_t put_msg, void *user_data);

/*! \brief Receive a chunk of ADSI audio.
    \param s The ADSI receive context.
    \param amp The audio sample buffer.
    \param len The number of samples in the buffer.
*/
void adsi_rx(adsi_rx_state_t *s, const int16_t *amp, int len);

/*! \brief Initialise an ADSI transmit context.
    \param s The ADSI transmit context.
    \param standard The code for the ADSI standard to be used.
*/
void adsi_tx_init(adsi_tx_state_t *s, int standard);

/*! \brief Generate a block of ADSI audio samples.
    \param s The ADSI transmit context.
    \param amp The audio sample buffer.
    \param max_len The number of samples to be generated.
    \return The number of samples actually generated.
*/
int adsi_tx(adsi_tx_state_t *s, int16_t *amp, int max_len);

/*! \brief Request generation of an ADSI alert tone.
    \param s The ADSI transmit context.
*/
void adsi_send_alert_tone(adsi_tx_state_t *s);

/*! \brief Put a message into the input buffer of an ADSI transmit context.
    \param s The ADSI transmit context.
    \param msg The message.
    \param len The length of the message.
    \return The length actually added. If a message is already in progress
            in the transmitter, this function will return zero, as it will
            not successfully add the message to the buffer.
*/
int adsi_put_message(adsi_tx_state_t *s, uint8_t *msg, int len);

/*! \brief Get a field from an ADSI message.
    \param s The ADSI receive context.
    \param msg The message buffer.
    \param msg_len The length of the message.
    \param pos Current position within the message. Set to -1 when starting a message.
    \param field_type The type code for the field.
    \param field_body Pointer to the body of the field.
    \param field_len The length of the field, or -1 for no more fields, or -2 for message structure corrupt.
*/
int adsi_next_field(adsi_rx_state_t *s, const uint8_t *msg, int msg_len, int pos, uint8_t *field_type, uint8_t const **field_body, int *field_len);

/*! \brief Insert the header or a field into an ADSI message.
    \param s The ADSI transmit context.
    \param msg The message buffer.
    \param len The current length of the message.
    \param field_type The type code for the new field.
    \param field_body Pointer to the body of the new field.
    \param field_len The length of the new field.
*/
int adsi_add_field(adsi_tx_state_t *s, uint8_t *msg, int len, uint8_t field_type, uint8_t const *field_body, int field_len);

/*! \brief Return a short name for an ADSI standard
    \param standard The code for the standard.
    \return A pointer to the name.
*/
const char *adsi_standard_to_str(int standard);

#ifdef __cplusplus
}
#endif

#endif
/*- End of file ------------------------------------------------------------*/
