/*
 * SpanDSP - a series of DSP components for telephony
 *
 * t38_gateway_tests.c - Tests for the T.38 FoIP gateway module.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2005, 2006 Steve Underwood
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
 * $Id: t38_gateway_tests.c,v 1.46 2007/03/29 12:28:37 steveu Exp $
 */

/*! \file */

/*! \page t38_gateway_tests_page T.38 gateway tests
\section t38_gateway_tests_page_sec_1 What does it do?
These tests exercise the path

    FAX machine <-> T.38 gateway <-> T.38 gateway <-> FAX machine
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined(HAVE_FL_FL_H)  &&  defined(HAVE_FL_FL_CARTESIAN_H)  &&  defined(HAVE_FL_FL_AUDIO_METER_H)
#define ENABLE_GUI
#endif

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#if defined(HAVE_TGMATH_H)
#include <tgmath.h>
#endif
#if defined(HAVE_MATH_H)
#include <math.h>
#endif
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#if !defined(__USE_MISC)
#define __USE_MISC
#endif
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <audiofile.h>
#include <tiffio.h>

#include "spandsp.h"
#include "g1050.h"
#if defined(ENABLE_GUI)
#include "media_monitor.h"
#endif

#define SAMPLES_PER_CHUNK       160

#define INPUT_FILE_NAME         "../itutests/fax/itutests.tif"
#define OUTPUT_FILE_NAME        "t38.tif"
#define OUTPUT_FILE_NAME_WAVE   "t38_gateway.wav"
#define OUTPUT_FILE_NAME_T30A   "t38_gateway_t30a.wav"
#define OUTPUT_FILE_NAME_T38A   "t38_gateway_t38a.wav"
#define OUTPUT_FILE_NAME_T30B   "t38_gateway_t30b.wav"
#define OUTPUT_FILE_NAME_T38B   "t38_gateway_t38b.wav"

fax_state_t fax_state_a;
t38_gateway_state_t t38_state_a;
t38_gateway_state_t t38_state_b;
fax_state_t fax_state_b;

g1050_state_t *path_a_to_b;
g1050_state_t *path_b_to_a;

double when = 0.0;

int done[2] = {FALSE, FALSE};
int succeeded[2] = {FALSE, FALSE};

int simulate_incrementing_repeats = FALSE;

static void phase_b_handler(t30_state_t *s, void *user_data, int result)
{
    int i;
    
    i = (int) (intptr_t) user_data;
    printf("%c: Phase B handler on channel %c - (0x%X) %s\n", i, i, result, t30_frametype(result));
}
/*- End of function --------------------------------------------------------*/

static void phase_d_handler(t30_state_t *s, void *user_data, int result)
{
    int i;
    t30_stats_t t;
    char ident[21];

    i = (int) (intptr_t) user_data;
    printf("%c: Phase D handler on channel %c - (0x%X) %s\n", i, i, result, t30_frametype(result));
    t30_get_transfer_statistics(s, &t);
    printf("%c: Phase D: bit rate %d\n", i, t.bit_rate);
    printf("%c: Phase D: ECM %s\n", i, (t.error_correcting_mode)  ?  "on"  :  "off");
    printf("%c: Phase D: pages transferred %d\n", i, t.pages_transferred);
    printf("%c: Phase D: image size %d x %d\n", i, t.width, t.length);
    printf("%c: Phase D: image resolution %d x %d\n", i, t.x_resolution, t.y_resolution);
    printf("%c: Phase D: bad rows %d\n", i, t.bad_rows);
    printf("%c: Phase D: longest bad row run %d\n", i, t.longest_bad_row_run);
    printf("%c: Phase D: coding method %s\n", i, t4_encoding_to_str(t.encoding));
    printf("%c: Phase D: image size %d\n", i, t.image_size);
    t30_get_local_ident(s, ident);
    printf("%c: Phase D: local ident '%s'\n", i, ident);
    t30_get_far_ident(s, ident);
    printf("%c: Phase D: remote ident '%s'\n", i, ident);
}
/*- End of function --------------------------------------------------------*/

static void phase_e_handler(t30_state_t *s, void *user_data, int result)
{
    int i;
    t30_stats_t t;
    char ident[21];
    
    i = (int) (intptr_t) user_data;
    printf("%c: Phase E handler on channel %c - (%d) %s\n", i, i, result, t30_completion_code_to_str(result));
    t30_get_transfer_statistics(s, &t);
    printf("%c: Phase E: bit rate %d\n", i, t.bit_rate);
    printf("%c: Phase E: ECM %s\n", i, (t.error_correcting_mode)  ?  "on"  :  "off");
    printf("%c: Phase E: pages transferred %d\n", i, t.pages_transferred);
    printf("%c: Phase E: image size %d x %d\n", i, t.width, t.length);
    printf("%c: Phase E: image resolution %d x %d\n", i, t.x_resolution, t.y_resolution);
    printf("%c: Phase E: bad rows %d\n", i, t.bad_rows);
    printf("%c: Phase E: longest bad row run %d\n", i, t.longest_bad_row_run);
    printf("%c: Phase E: coding method %s\n", i, t4_encoding_to_str(t.encoding));
    printf("%c: Phase E: image size %d bytes\n", i, t.image_size);
    t30_get_local_ident(s, ident);
    printf("%c: Phase E: local ident '%s'\n", i, ident);
    t30_get_far_ident(s, ident);
    printf("%c: Phase E: remote ident '%s'\n", i, ident);
    succeeded[i - 'A'] = (result == T30_ERR_OK)  &&  (t.pages_transferred == 12);
    done[i - 'A'] = TRUE;
}
/*- End of function --------------------------------------------------------*/

static int tx_packet_handler_a(t38_core_state_t *s, void *user_data, const uint8_t *buf, int len, int count)
{
    t38_terminal_state_t *t;
    int i;
    static int subst_seq = 0;

    /* This routine queues messages between two instances of T.38 processing */
    t = (t38_terminal_state_t *) user_data;
    if (simulate_incrementing_repeats)
    {
        for (i = 0;  i < count;  i++)
        {
            span_log(&s->logging, SPAN_LOG_FLOW, "Send seq %d, len %d\n", subst_seq, len);

            g1050_put(path_a_to_b, buf, len, subst_seq, when);
            subst_seq = (subst_seq + 1) & 0xFFFF;
        }
    }
    else
    {
        span_log(&s->logging, SPAN_LOG_FLOW, "Send seq %d, len %d, count %d\n", s->tx_seq_no, len, count);

        for (i = 0;  i < count;  i++)
            g1050_put(path_a_to_b, buf, len, s->tx_seq_no, when);
    }
    return 0;
}
/*- End of function --------------------------------------------------------*/

static int tx_packet_handler_b(t38_core_state_t *s, void *user_data, const uint8_t *buf, int len, int count)
{
    t38_terminal_state_t *t;
    int i;
    static int subst_seq = 0;

    /* This routine queues messages between two instances of T.38 processing */
    t = (t38_terminal_state_t *) user_data;
    if (simulate_incrementing_repeats)
    {
        for (i = 0;  i < count;  i++)
        {
            span_log(&s->logging, SPAN_LOG_FLOW, "Send seq %d, len %d\n", subst_seq, len);

            g1050_put(path_b_to_a, buf, len, subst_seq, when);
            subst_seq = (subst_seq + 1) & 0xFFFF;
        }
    }
    else
    {
        span_log(&s->logging, SPAN_LOG_FLOW, "Send seq %d, len %d, count %d\n", s->tx_seq_no, len, count);

        for (i = 0;  i < count;  i++)
            g1050_put(path_b_to_a, buf, len, s->tx_seq_no, when);
    }
    return 0;
}
/*- End of function --------------------------------------------------------*/

int main(int argc, char *argv[])
{
    int16_t silence[SAMPLES_PER_CHUNK];
    int16_t t30_amp_a[SAMPLES_PER_CHUNK];
    int16_t t38_amp_a[SAMPLES_PER_CHUNK];
    int16_t t38_amp_b[SAMPLES_PER_CHUNK];
    int16_t t30_amp_b[SAMPLES_PER_CHUNK];
    int16_t out_amp[SAMPLES_PER_CHUNK*4];
    int t30_len_a;
    int t38_len_a;
    int t38_len_b;
    int t30_len_b;
    int log_audio;
    int msg_len;
    uint8_t msg[1024];
    int outframes;
    AFfilesetup filesetup;
    AFfilehandle wave_handle;
    int use_ecm;
    int t38_version;
    const char *input_file_name;
    int i;
    int seq_no;
    int model_no;
    int speed_pattern_no;
    double tx_when;
    double rx_when;
    int use_gui;

    log_audio = FALSE;
    use_ecm = FALSE;
    t38_version = 1;
    input_file_name = INPUT_FILE_NAME;
    simulate_incrementing_repeats = FALSE;
    model_no = 0;
    speed_pattern_no = 1;
    use_gui = FALSE;
    for (i = 1;  i < argc;  i++)
    {
        if (strcmp(argv[i], "-e") == 0)
        {
            use_ecm = TRUE;
            continue;
        }
        if (strcmp(argv[i], "-g") == 0)
        {
            use_gui = TRUE;
            continue;
        }
        if (strcmp(argv[i], "-i") == 0)
        {
            input_file_name = argv[++i];
            continue;
        }
        if (strcmp(argv[i], "-I") == 0)
        {
            simulate_incrementing_repeats = TRUE;
            continue;
        }
        if (strcmp(argv[i], "-l") == 0)
        {
            log_audio = TRUE;
            continue;
        }
        if (strcmp(argv[i], "-m") == 0)
        {
            model_no = argv[++i][0] - 'A' + 1;
            continue;
        }
        if (strcmp(argv[i], "-s") == 0)
        {
            speed_pattern_no = atoi(argv[++i]);
            continue;
        }
        if (strcmp(argv[i], "-v") == 0)
        {
            t38_version = atoi(argv[++i]);
            continue;
        }
    }

    printf("Using T.38 version %d\n", t38_version);
    if (use_ecm)
        printf("Using ECM\n");

    filesetup = AF_NULL_FILESETUP;
    wave_handle = AF_NULL_FILEHANDLE;
    if (log_audio)
    {
        if ((filesetup = afNewFileSetup()) == AF_NULL_FILESETUP)
        {
            fprintf(stderr, "    Failed to create file setup\n");
            exit(2);
        }
        afInitSampleFormat(filesetup, AF_DEFAULT_TRACK, AF_SAMPFMT_TWOSCOMP, 16);
        afInitRate(filesetup, AF_DEFAULT_TRACK, (float) SAMPLE_RATE);
        afInitFileFormat(filesetup, AF_FILE_WAVE);
        afInitChannels(filesetup, AF_DEFAULT_TRACK, 4);
        if ((wave_handle = afOpenFile(OUTPUT_FILE_NAME_WAVE, "w", filesetup)) == AF_NULL_FILEHANDLE)
        {
            fprintf(stderr, "    Cannot create wave file '%s'\n", OUTPUT_FILE_NAME_WAVE);
            exit(2);
        }
    }
    memset(silence, 0, sizeof(silence));
 
    srand48(0x1234567);
    if ((path_a_to_b = g1050_init(model_no, speed_pattern_no, 100, 33)) == NULL)
    {
        fprintf(stderr, "Failed to start IP network path model\n");
        exit(2);
    }
    if ((path_b_to_a = g1050_init(model_no, speed_pattern_no, 100, 33)) == NULL)
    {
        fprintf(stderr, "Failed to start IP network path model\n");
        exit(2);
    }

    fax_init(&fax_state_a, TRUE);
    //t30_set_supported_modems(&(fax_state_a.t30_state), T30_SUPPORT_V27TER | T30_SUPPORT_V29);
    t30_set_local_ident(&fax_state_a.t30_state, "11111111");
    t30_set_tx_file(&fax_state_a.t30_state, input_file_name, -1, -1);
    t30_set_phase_b_handler(&fax_state_a.t30_state, phase_b_handler, (void *) (intptr_t) 'A');
    t30_set_phase_d_handler(&fax_state_a.t30_state, phase_d_handler, (void *) (intptr_t) 'A');
    t30_set_phase_e_handler(&fax_state_a.t30_state, phase_e_handler, (void *) (intptr_t) 'A');
    t30_set_ecm_capability(&fax_state_a.t30_state, use_ecm);
    if (use_ecm)
        t30_set_supported_compressions(&fax_state_a.t30_state, T30_SUPPORT_T4_1D_COMPRESSION | T30_SUPPORT_T4_2D_COMPRESSION | T30_SUPPORT_T6_COMPRESSION);
    span_log_set_level(&fax_state_a.logging, SPAN_LOG_DEBUG | SPAN_LOG_SHOW_TAG | SPAN_LOG_SHOW_SAMPLE_TIME);
    span_log_set_tag(&fax_state_a.logging, "FAX-A ");
    span_log_set_level(&fax_state_a.t30_state.logging, SPAN_LOG_DEBUG | SPAN_LOG_SHOW_TAG | SPAN_LOG_SHOW_SAMPLE_TIME);
    span_log_set_tag(&fax_state_a.t30_state.logging, "FAX-A ");
    memset(t30_amp_a, 0, sizeof(t30_amp_a));

    if (t38_gateway_init(&t38_state_a, tx_packet_handler_a, &t38_state_b) == NULL)
    {
        fprintf(stderr, "Cannot start the T.38 channel\n");
        exit(2);
    }
    t38_set_t38_version(&t38_state_a.t38, t38_version);
    t38_gateway_ecm_control(&t38_state_a, use_ecm);
    span_log_set_level(&t38_state_a.logging, SPAN_LOG_DEBUG | SPAN_LOG_SHOW_TAG | SPAN_LOG_SHOW_SAMPLE_TIME);
    span_log_set_tag(&t38_state_a.logging, "T.38-A");
    span_log_set_level(&t38_state_a.t38.logging, SPAN_LOG_DEBUG | SPAN_LOG_SHOW_TAG | SPAN_LOG_SHOW_SAMPLE_TIME);
    span_log_set_tag(&t38_state_a.t38.logging, "T.38-A");
    memset(t38_amp_a, 0, sizeof(t38_amp_a));

    if (t38_gateway_init(&t38_state_b, tx_packet_handler_b, &t38_state_a) == NULL)
    {
        fprintf(stderr, "Cannot start the T.38 channel\n");
        exit(2);
    }
    t38_set_t38_version(&t38_state_b.t38, t38_version);
    t38_gateway_ecm_control(&t38_state_b, use_ecm);
    span_log_set_level(&t38_state_b.logging, SPAN_LOG_DEBUG | SPAN_LOG_SHOW_TAG | SPAN_LOG_SHOW_SAMPLE_TIME);
    span_log_set_tag(&t38_state_b.logging, "T.38-B");
    span_log_set_level(&t38_state_b.t38.logging, SPAN_LOG_DEBUG | SPAN_LOG_SHOW_TAG | SPAN_LOG_SHOW_SAMPLE_TIME);
    span_log_set_tag(&t38_state_b.t38.logging, "T.38-B");
    memset(t38_amp_b, 0, sizeof(t38_amp_b));

    fax_init(&fax_state_b, FALSE);
    t30_set_local_ident(&fax_state_b.t30_state, "22222222");
    t30_set_rx_file(&fax_state_b.t30_state, OUTPUT_FILE_NAME, -1);
    t30_set_phase_b_handler(&fax_state_b.t30_state, phase_b_handler, (void *) (intptr_t) 'B');
    t30_set_phase_d_handler(&fax_state_b.t30_state, phase_d_handler, (void *) (intptr_t) 'B');
    t30_set_phase_e_handler(&fax_state_b.t30_state, phase_e_handler, (void *) (intptr_t) 'B');
    t30_set_ecm_capability(&fax_state_b.t30_state, use_ecm);
    if (use_ecm)
        t30_set_supported_compressions(&fax_state_b.t30_state, T30_SUPPORT_T4_1D_COMPRESSION | T30_SUPPORT_T4_2D_COMPRESSION | T30_SUPPORT_T6_COMPRESSION);
    span_log_set_level(&fax_state_b.logging, SPAN_LOG_DEBUG | SPAN_LOG_SHOW_TAG | SPAN_LOG_SHOW_SAMPLE_TIME);
    span_log_set_tag(&fax_state_b.logging, "FAX-B ");
    span_log_set_level(&fax_state_b.t30_state.logging, SPAN_LOG_DEBUG | SPAN_LOG_SHOW_TAG | SPAN_LOG_SHOW_SAMPLE_TIME);
    span_log_set_tag(&fax_state_b.t30_state.logging, "FAX-B ");
    memset(t30_amp_b, 0, sizeof(t30_amp_b));

#if defined(ENABLE_GUI)
    if (use_gui)
        start_media_monitor();
#endif
    for (;;)
    {
        span_log_bump_samples(&fax_state_a.logging, SAMPLES_PER_CHUNK);
        span_log_bump_samples(&fax_state_a.t30_state.logging, SAMPLES_PER_CHUNK);
        span_log_bump_samples(&t38_state_a.logging, SAMPLES_PER_CHUNK);
        span_log_bump_samples(&t38_state_a.t38.logging, SAMPLES_PER_CHUNK);
        span_log_bump_samples(&t38_state_b.logging, SAMPLES_PER_CHUNK);
        span_log_bump_samples(&t38_state_b.t38.logging, SAMPLES_PER_CHUNK);
        span_log_bump_samples(&fax_state_b.logging, SAMPLES_PER_CHUNK);
        span_log_bump_samples(&fax_state_b.t30_state.logging, SAMPLES_PER_CHUNK);
        memset(out_amp, 0, sizeof(out_amp));

        t30_len_a = fax_tx(&fax_state_a, t30_amp_a, SAMPLES_PER_CHUNK);
        /* The receive side always expects a full block of samples, but the
           transmit side may not be sending any when it doesn't need to. We
           may need to pad with some silence. */
        if (t30_len_a < SAMPLES_PER_CHUNK)
        {
            memset(t30_amp_a + t30_len_a, 0, sizeof(int16_t)*(SAMPLES_PER_CHUNK - t30_len_a));
            t30_len_a = SAMPLES_PER_CHUNK;
        }
        if (log_audio)
        {
            for (i = 0;  i < t30_len_a;  i++)
                out_amp[i*4] = t30_amp_a[i];
        }
        if (t38_gateway_rx(&t38_state_a, t30_amp_a, t30_len_a))
            break;

        t38_len_a = t38_gateway_tx(&t38_state_a, t38_amp_a, SAMPLES_PER_CHUNK);
        if (t38_len_a < SAMPLES_PER_CHUNK)
        {
            memset(t38_amp_a + t38_len_a, 0, sizeof(int16_t)*(SAMPLES_PER_CHUNK - t38_len_a));
            t38_len_a = SAMPLES_PER_CHUNK;
        }
        if (log_audio)
        {
            for (i = 0;  i < t38_len_a;  i++)
                out_amp[i*4 + 1] = t38_amp_a[i];
        }
        if (fax_rx(&fax_state_a, t38_amp_a, SAMPLES_PER_CHUNK))
            break;

        t30_len_b = fax_tx(&fax_state_b, t30_amp_b, SAMPLES_PER_CHUNK);
        /* The receive side always expects a full block of samples, but the
           transmit side may not be sending any when it doesn't need to. We
           may need to pad with some silence. */
        if (t30_len_b < SAMPLES_PER_CHUNK)
        {
            memset(t30_amp_b + t30_len_b, 0, sizeof(int16_t)*(SAMPLES_PER_CHUNK - t30_len_b));
            t30_len_b = SAMPLES_PER_CHUNK;
        }
        if (log_audio)
        {
            for (i = 0;  i < t30_len_b;  i++)
                out_amp[i*4 + 3] = t30_amp_b[i];
        }
        if (t38_gateway_rx(&t38_state_b, t30_amp_b, t30_len_b))
            break;

        t38_len_b = t38_gateway_tx(&t38_state_b, t38_amp_b, SAMPLES_PER_CHUNK);
        if (t38_len_b < SAMPLES_PER_CHUNK)
        {
            memset(t38_amp_b + t38_len_b, 0, sizeof(int16_t)*(SAMPLES_PER_CHUNK - t38_len_b));
            t38_len_b = SAMPLES_PER_CHUNK;
        }
        if (log_audio)
        {
            for (i = 0;  i < t38_len_b;  i++)
                out_amp[i*4 + 2] = t38_amp_b[i];
        }
        if (fax_rx(&fax_state_b, t38_amp_b, SAMPLES_PER_CHUNK))
            break;

        when += 0.02;

        while ((msg_len = g1050_get(path_a_to_b, msg, 1024, when, &seq_no, &tx_when, &rx_when)) >= 0)
        {
#if defined(ENABLE_GUI)
            if (use_gui)
                media_monitor_rx(seq_no, tx_when, rx_when);
#endif
            t38_core_rx_ifp_packet(&t38_state_b.t38, seq_no, msg, msg_len);
        }
        while ((msg_len = g1050_get(path_b_to_a, msg, 1024, when, &seq_no, &tx_when, &rx_when)) >= 0)
        {
#if defined(ENABLE_GUI)
            if (use_gui)
                media_monitor_rx(seq_no, tx_when, rx_when);
#endif
            t38_core_rx_ifp_packet(&t38_state_a.t38, seq_no, msg, msg_len);
        }
        if (log_audio)
        {
            outframes = afWriteFrames(wave_handle, AF_DEFAULT_TRACK, out_amp, SAMPLES_PER_CHUNK);
            if (outframes != SAMPLES_PER_CHUNK)
                break;
        }

        if (done[0]  &&  done[1])
            break;
#if defined(ENABLE_GUI)
        if (use_gui)
            media_monitor_update_display();
#endif
    }
    if (log_audio)
    {
        if (afCloseFile(wave_handle) != 0)
        {
            fprintf(stderr, "    Cannot close wave file '%s'\n", OUTPUT_FILE_NAME_WAVE);
            exit(2);
        }
        afFreeFileSetup(filesetup);
    }
    if (!succeeded[0]  ||  !succeeded[1])
    {
        printf("Tests failed\n");
        exit(2);
    }
    printf("Tests passed\n");
    return  0;
}
/*- End of function --------------------------------------------------------*/
/*- End of file ------------------------------------------------------------*/
