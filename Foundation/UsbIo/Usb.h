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

#ifndef _CORE_H_
#define _CORE_H_

#include <Core.h>

#ifdef __linux__
#include <libusb-1.0/libusb.h>
#endif

#define SERIAL_SIZE     256

/*
 * Device communications structure
 */

typedef struct __usb_device_context {
#ifdef __linux__
    libusb_device_handle device_handle;
#else
    void* device_handle;
#endif
    uint32_t device_interface;
    uint32_t device_alternate_interface;
    uint32_t device_pid;
    uint32_t device_vid;
    uint32_t device_configuration;
    char device_serial[SERIAL_SIZE];
} usb_device_context_t;

/*
 * Device descriptor constants
 */

#define VENDOR_APPLE            0x5AC
#define TARGET_VENDOR           VENDOR_APPLE
#define TARGET_DEVICE_DFU       0x1227
#define TARGET_DEVICE_IBOOT     0x1281

#endif
