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
 * Default debugging level. 0 is nothing.
 */

static uint32_t __usb_debugging_level = 0;

/*
 * DO NOT USE THIS DIRECTLY IN YOUR PROGRAMS
 * VERY BAD THINGS MAY HAPPEN IF YOU PERFORM MALICIOUS ACTIONS.
 *
 * (Your warranty also expires if you do that. So don't do that. Please.)
 */
usb_device_context_t context;

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
    USB_DPRINTF("Setting configuration to %d\n", config_num);

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
    USB_DPRINTF("Setting interface to: (%d, %d)\n", intf, alt_intf);

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

/*
 * Open/close device.
 */

static int is_initialized = 0;

int usb_device_try_open(uint32_t required_device, int times)
{
    int i;
    USB_DPRINTF("Polling for pid: %x, times: %d\n", required_device, 
                times);
    for(i = 0; i < times; i++) {
        if(usb_device_open(required_device) == 0) {
            return 0;
        }
        USB_DPRINTF("Failed to find device; waiting 1 second ...\n");
        sleep(1);
    }
    return 1;
}

int usb_device_open(uint32_t required_device) {
    struct libusb_device *usb_device;
    struct libusb_device **usb_device_list;
    struct libusb_device_handle* usb_handle;
    struct libusb_device_descriptor usb_device_descriptor;
    int usb_device_count, i;

    if(is_initialized == 0) {
        libusb_init(&context.device_context);
        is_initialized = 1;
    }

    USB_DPRINTF("Opening device with requested pid: %x\n", required_device);

    usb_device_count = libusb_get_device_list(context.device_context, &usb_device_list);
    for(i = 0; i < usb_device_count; i++) {
        usb_device = (usb_device_list[i]);
        libusb_get_device_descriptor(usb_device, &usb_device_descriptor);

         USB_DPRINTF("Found usb device with vidpid: %04X:%04X\n",
                     usb_device_descriptor.idVendor,
                     usb_device_descriptor.idProduct);

        if(usb_device_descriptor.idVendor == VENDOR_APPLE &&
           usb_device_descriptor.idProduct == required_device) {
            libusb_open(usb_device, &usb_handle);
            if(usb_handle == NULL) {
                errx(1, "can't connect to device");
            }

            memset(&context, 0, sizeof(usb_device_context_t));

            context.device_interface = 0;
            context.device_handle = usb_handle;
            context.device_vid = usb_device_descriptor.idVendor;
            context.device_pid = usb_device_descriptor.idProduct;

            usb_device_get_string_descriptor(usb_device_descriptor.iSerialNumber,
                                      context.device_serial, 255);

            USB_DPRINTF("Device serial: %s\n", context.device_serial);

            usb_device_set_configuration(1);

            usb_device_set_interface(0, 0);
            if(context.device_pid == TARGET_DEVICE_IBOOT)
                usb_device_set_interface(1, 1);

            return 0;
        }
    }
    return -1;
}

int usb_device_close(void)
{
    libusb_close(context.device_handle);
    memset(&context, 0, sizeof(context));

    if(is_initialized == 1) {
        libusb_exit(context.device_context);
        is_initialized = 0;
    }

    return 0;
}

/*
 * iBoot commands.
 */

int usb_device_send_command(const char* command)
{
    int length = strlen(command);
    usb_control_packet_t packet;

    if(length >= 0x100)
        length = 0xFF;

    if(length) {
        MakeRequest(packet, 0x40, 0, 0, 0, (uint8_t*)command, length + 1);
        usb_device_control_transfer(&packet, 1000);
    }

    return 0;
}

/*
 * File send/transmit. Kermit?
 */

int usb_device_send_buffer(uint8_t* buffer, uint32_t size, uint32_t dfu_notify)
{
    int recovery_mode = 0;
    int packet_size = 0x800;
    int last = size % packet_size;
    int packets = size / packet_size;
    int res, transmitted = 0, i;
    usb_control_packet_t packet;

    if(last != 0)
        packets++;
    else
        last = packet_size;

    if(context.device_pid == TARGET_DEVICE_IBOOT)
        recovery_mode = 1;

    /*
     * Start transfer.
     */
    if(recovery_mode) {
        MakeRequest(packet, 0x41, 0, 0, 0, NULL, 0);
        res = usb_device_control_transfer(&packet, 1000);
    } else {
        MakeRequest(packet, 0x21, 4, 0, 0, NULL, 0);
        res = usb_device_control_transfer(&packet, 1000);
    }

    USB_DPRINTF("Preparing transfer %d\n", res);

    if(res != 0)
        return -1;

    USB_DPRINTF("Sending packets\n");

    double count = 0; double progress = 0;
    for(i = 0; i < packets; i++) {
        int __size = (i + 1) < packets ? packet_size : last;
        int status = 0;

        if(recovery_mode) {
            usb_device_bulk_transfer(0x04, &buffer[i * packet_size], __size,
                                     &transmitted, 1000);
        } else {
            MakeRequest(packet, 0x21, 1, 0, 0, &buffer[i * packet_size], __size);
            transmitted = usb_device_control_transfer(&packet, 1000);
        }

        if(__size != transmitted) {
            warnx("failure to transmit bytes (%d != %d)", __size, transmitted);
            return -1;
        }

        if(!recovery_mode) {
            uint8_t buf[6];
            memset(buf, 0, 6);
            MakeRequest(packet, 0xA1, 3, 0, 0, buf, 6);
            if(usb_device_control_transfer(&packet, 1000) != 6)
                status = 0;
            status = buf[4];
        }

        if((!recovery_mode) && (status != 5)) {
            warnx("failure to upload, status: %d", status);
            return -1;
        }

        count += transmitted;
        char cursor[4] = {'/', '-', '\\', '|'};
        static int pos = 0;
        if((count / (double)size) * 100.0 > progress) {
            progress = (count / (double)size) * 100.0;
            printf("%3.1f%% done uploading - %c\r", progress, cursor[pos]);
            fflush(stdout);
            pos = (pos + 1) % 4;
        }
        if(progress == 100)
            printf("\n");
    }

    if(dfu_notify && !recovery_mode) {
        MakeRequest(packet, 0x21, 1, 0, 0, (uint8_t*)buffer, 0);
        usb_device_control_transfer(&packet, 1000);
        for(i = 0; i < 3; i++) {
            uint8_t buf[6];
            memset(buf, 0, 6);
            MakeRequest(packet, 0xA1, 3, 0, 0, buf, 6);
            usb_device_control_transfer(&packet, 1000);
        }
        usb_device_reset();
    }

    return 0;
}

