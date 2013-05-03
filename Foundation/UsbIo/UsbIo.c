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
#include "Usb.h"

/*
 * Default debugging level. 0 is nothing.
 */

static uint32_t __usb_debugging_level = 0;
static usb_device_context_t context;

/*
 * USB debugging handler functionality.
 */
uint32_t usb_get_debugging_level(void) 
{
    return __usb_debugging_level;
}

void usb_set_debugging_level(uint32_t debug_level) 
{
    __usb_debugging_level = debug_level;
    return;
}

/*
 * USB API
 */
int usb_device_control_transfer(usb_control_packet_t* packet,
                                uint32_t timeout)
{
    return libusb_control_transfer(context.device_handle,
                                   packet->bmRequestType,
                                   packet->bRequest,
                                   packet->wValue,
                                   packet->wIndex,
                                   packet->data,
                                   packet->wLength,
                                   timeout);
}

int usb_device_bulk_transfer(uint8_t endpoint, uint8_t* data,
                             uint32_t length, int32_t *transferred,
                             uint32_t timeout)
{
    int i;

    i = libusb_bulk_transfer(context.device_handle, endpoint, data,
                             length, transferred, timeout);

    if(i < 0)
        libusb_clear_halt(context.device_handle, endpoint);

    return i;
}

int usb_device_get_string_descriptor(uint8_t index, uint8_t* buffer, uint32_t length)
{
    if(buffer == NULL)
        return -1;

    return libusb_get_string_descriptor_ascii(context.device_handle, index, buffer, length);
}

int usb_device_set_configuration(uint32_t config_num)
{
    int current_config = 0;
    libusb_get_configuration(context.device_handle, &current_config);
    if(current_config != config_num) {
        if(libusb_set_configuration(context.device_handle, config_num) < 0)
            return -1;
    }
    context.device_configuration = config_num;
    return 0;
}

int usb_device_set_interface(uint32_t intf, uint32_t alt_intf)
{
    if(libusb_claim_interface(context.device_handle, intf) < 0)
        return -1;

    if(libusb_set_interface_alt_setting(context.device_handle, intf, alt_intf) < 0)
        return -1;

    context.device_interface = intf;
    context.device_alternate_interface = alt_intf;

    return 0;
}

int usb_device_reset(void)
{
    libusb_reset_device(context.device_handle);
    return 0;
}


