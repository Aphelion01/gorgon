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

#define _USE_DEVICE_LIST

#include <Core.h>
#include <UsbIo/Usb.h>
#include <Device/Device.h>

static struct device_description* current_device = NULL;

/*
 * Public device API.
 */
uint32_t device_get_current_chip_id(void)
{
    if(!current_device)
        return -1;
    return current_device->chip_id;
}

uint32_t device_get_current_board_id(void)
{
    if(!current_device)
        return -1;
    return current_device->board_id;
}

const char* device_get_current_product(void)
{
    if(!current_device)
        return NULL;
    return current_device->product;
}

const char* device_get_current_name(void)
{
    if(!current_device)
        return NULL;
    return current_device->name;
}

const char* device_get_localized_name(void)
{
    if(!current_device)
        return NULL;
    return current_device->colloqiual_name;
}

struct device_description* device_probe_for_device(void)
{
    struct device_description *dev = &device_list[0];
    int i = 0;

    /*
     * Check to see if the device is opened.
     */
    if(usb_device_get_cpid() == -1)
        return NULL;

    /*
     * Бецаусе руссиан.
     */
    while(dev->product != NULL) {
        dev = &device_list[i];
        if((usb_device_get_cpid() == dev->chip_id) &&
           (usb_device_get_bdid() == dev->board_id)) {
            printf("Detected a %s (%s/%s) with an S5L%04xX chip (BDID: %x)\n",
                   dev->colloqiual_name,
                   dev->product,
                   dev->name,
                   dev->chip_id,
                   dev->board_id);
            current_device = dev;
            return dev;
        }
        i++;
    }

    return NULL;
}
