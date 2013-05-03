/*-
 * Copyright (c) 2013, winocm. <rms@velocitylimitless.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <Core.h>
#include <UsbIo/Usb.h>

/*
 * USB device context 
 *
 * Please see warning in UsbIo.c about messing with this structure.
 */
extern usb_device_context_t context;

static char* usb_device_internal_get_serial(void)
{
    return (char*)context.device_serial;
}

static char* usb_device_internal_get_nonce(void)
{
    char nonce[256];
    usb_device_get_string_descriptor(1, (uint8_t*)&nonce, 256);
    return strdup(nonce);
}

/*
 * Nonce.
 */ 
int usb_device_get_nonce(uint32_t* buffer_size, uint8_t** buffer)
{
    char* nonce = usb_device_internal_get_nonce();
    uint8_t *p;
    int length, i;

    if(strstr(nonce, "NONC:") == NULL) {
        *buffer_size = 0;
        *buffer = 0;
        return -1;
    }

    nonce += 6;
    length = strlen(nonce);

    p = malloc(length / 2);
    for(i = 0; i < (length / 2); i++) {
        uint32_t val = 0;
        if(sscanf(nonce + (i * 2), "%02X", &val) == 1) {
            p[i] = (uint8_t)val;
        } else {
            warnx("weird data found in nonce?");
            break;
        }
    }

    if(i != (length / 2)) {
        free(p);
    }

    *buffer = p;
    *buffer_size = (length / 2);

    return 0;
}

/*
 * Buffer size should be 255 or greater.
 */
int usb_device_get_serial(uint32_t buffer_size, uint8_t* buffer)
{
    char *serial = NULL;
    if((buffer_size < 255) || (buffer == NULL))
        return -1;
    serial = usb_device_internal_get_serial();
    if(!serial)
        return -1;
    strncpy((char*)buffer, serial, buffer_size);
    return 0;
}

/*
 * DFU field identifiers
 */
uint32_t usb_device_get_cpid(void)
{
    char* cpid_string = strstr(usb_device_internal_get_serial(),
                               "CPID:");
    if(cpid_string) {
        uint32_t cpid;
        sscanf(cpid_string, "CPID:%x", &cpid);
        return cpid;
    }
    return -1;
}

uint32_t usb_device_get_ibfl(void)
{
    char* ibfl_string = strstr(usb_device_internal_get_serial(),
                               "IBFL:");
    if(ibfl_string) {
        uint32_t ibfl;
        sscanf(ibfl_string, "IBFL:%x", &ibfl);
        return ibfl;
    }
    return -1;
}

uint32_t usb_device_get_bdid(void)
{
    char* bdid_string = strstr(usb_device_internal_get_serial(),
                               "BDID:");
    if(bdid_string) {
        uint32_t bdid;
        sscanf(bdid_string, "BDID:%x", &bdid);
        return bdid;
    }
    return -1;
}

uint32_t usb_device_get_scep(void)
{
    char* scep_string = strstr(usb_device_internal_get_serial(),
                               "SCEP:");
    if(scep_string) {
        uint32_t scep;
        sscanf(scep_string, "SCEP:%x", &scep);
        return scep;
    }
    return -1;
}

uint32_t usb_device_get_cprv(void)
{
    char* cprv_string = strstr(usb_device_internal_get_serial(),
                               "CPRV:");
    if(cprv_string) {
        uint32_t cprv;
        sscanf(cprv_string, "CPRV:%x", &cprv);
        return cprv;
    }
    return -1;
}

uint32_t usb_device_get_cpfm(void)
{
    char* cpfm_string = strstr(usb_device_internal_get_serial(),
                               "CPFM:");
    if(cpfm_string) {
        uint32_t cpfm;
        sscanf(cpfm_string, "CPFM:%x", &cpfm);
        return cpfm;
    }
    return -1;
}

uint64_t usb_device_get_ecid(void)
{
    char* ecid_string = strstr(usb_device_internal_get_serial(),
                               "ECID:");
    if(ecid_string) {
        uint64_t ecid;
        sscanf(ecid_string, "ECID:%qX", &ecid);
        return ecid;
    }
    return -1;
}

/*
 * iBoot string functions
 */

char* usb_device_get_srtg(void)
{
    char* srtg_string = strstr(usb_device_internal_get_serial(),
                               "SRTG:");
    if(srtg_string) {
        char srtg[256], *p;
        sscanf(srtg_string, "SRTG:[%s]", srtg);
        p = strrchr(srtg, ']');
        if((p = strrchr(srtg, ']')) != NULL)
            *p = '\0';
        return strdup(srtg);
    }
    return NULL;
}

char* usb_device_get_srnm(void)
{
    char* srnm_string = strstr(usb_device_internal_get_serial(),
                               "SRNM:");
    if(srnm_string) {
        char srnm[256], *p;
        sscanf(srnm_string, "SRNM:[%s]", srnm);
        p = strrchr(srnm, ']');
        if((p = strrchr(srnm, ']')) != NULL)
            *p = '\0';
        return strdup(srnm);
    }
    return NULL;
}

char* usb_device_get_imei(void)
{
    char* imei_string = strstr(usb_device_internal_get_serial(),
                               "IMEI:");
    if(imei_string) {
        char imei[256], *p;
        sscanf(imei_string, "IMEI:[%s]", imei);
        p = strrchr(imei, ']');
        if((p = strrchr(imei, ']')) != NULL)
            *p = '\0';
        return strdup(imei);
    }
    return NULL;
}

