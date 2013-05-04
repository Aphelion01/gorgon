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

#ifndef _USB_H_
#define _USB_H_

#include <Core.h>
#include <libusb-1.0/libusb.h>

#define SERIAL_SIZE     256

/*
 * Device communications structure
 */

typedef struct __usb_device_context {
    libusb_device_handle *device_handle;
    libusb_context *device_context;
    uint32_t device_interface;
    uint32_t device_alternate_interface;
    uint32_t device_pid;
    uint32_t device_vid;
    uint32_t device_configuration;
    uint8_t device_serial[SERIAL_SIZE];
} usb_device_context_t;

/*
 * USB packet structures.
 */

typedef struct __usb_control_packet {
    uint8_t bmRequestType;      /*
                                 * Hungarian notation is used to parallel
                                 * the original USB headers.
                                 */
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint8_t* data;
    uint16_t wLength;
} usb_control_packet_t;

#define MakeRequest(packet, _bmRequestType, _bRequest, _wValue, _wIndex, _data, _wLength) \
    {                                           \
        packet.bmRequestType = _bmRequestType;  \
        packet.bRequest = _bRequest;            \
        packet.wValue = _wValue;                \
        packet.wIndex = _wIndex;                \
        packet.data = _data;                    \
        packet.wLength = _wLength;              \
    }

/*
 * Device descriptor constants
 */

#define VENDOR_APPLE            0x5AC
#define TARGET_VENDOR           VENDOR_APPLE
#define TARGET_DEVICE_DFU       0x1227
#define TARGET_DEVICE_IBOOT     0x1281

/*
 * Debugging constructs.
 */

#ifndef __RELEASE__
#define USB_DPRINTF     printf("(%s:%d) ", __FILE__, __LINE__), printf
#else
#define USB_DPRINTF
#endif

/*
 * API.
 */
extern uint32_t usb_get_debugging_level __P((void));
extern void usb_set_debugging_level __P((uint32_t));

/* 
 * Device control.
 */

extern int usb_device_control_transfer __P((usb_control_packet_t*, uint32_t));
extern int usb_device_bulk_transfer __P((uint8_t, uint8_t*, uint32_t, int32_t*, uint32_t));
extern int usb_device_get_string_descriptor __P((uint8_t, uint8_t*, uint32_t));
extern int usb_device_set_configuration __P((uint32_t));
extern int usb_device_set_interface __P((uint32_t, uint32_t));
extern int usb_device_reset __P((void));

/*
 * Open/close API
 */

extern int usb_device_open __P((uint32_t));
extern int usb_device_close __P((void));
extern int usb_device_try_open __P((uint32_t, int));

/*
 * Device information API
 */

extern uint32_t usb_device_get_cpid __P((void));
extern uint32_t usb_device_get_bdid __P((void));
extern uint32_t usb_device_get_scep __P((void));
extern uint32_t usb_device_get_cprv __P((void));
extern uint32_t usb_device_get_cpfm __P((void));
extern uint32_t usb_device_get_ibfl __P((void));
extern uint64_t usb_device_get_ecid __P((void));

/*
 * usb_device_get_serial() requires a buffer with length 255
 * or greater.
 */
extern int usb_device_get_serial __P((uint32_t, uint8_t*));

/*
 * The strings returned by these functions can/must be freed.
 */

extern char* usb_device_get_srtg __P((void));
extern char* usb_device_get_srnm __P((void));
extern char* usb_device_get_imei __P((void));

extern int usb_device_get_nonce __P((uint32_t*, uint8_t**));

/*
 * iBoot crap.
 */

extern int usb_device_send_buffer __P((uint8_t*, uint32_t, uint32_t));
extern int usb_device_send_command __P((const char*));


#endif
