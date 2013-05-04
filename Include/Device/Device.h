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

#ifndef _DEVICE_H_
#define _DEVICE_H_

/*
 * Boot ROM versions
 */
#define BOOTROM_VERSION_S5L8720     "iBoot-240.4"
#define BOOTROM_VERSION_S5L8720_NEW "iBoot-240.5.1"
#define BOOTROM_VERSION_S5L8920     "iBoot-359.3"
#define BOOTROM_VERSION_S5L8920_NEW "iBoot-359.3.2"
#define BOOTROM_VERSION_S5L8922     "iBoot-359.5"
#define BOOTROM_VERSION_S5L8930     "iBoot-574.4"
#define BOOTROM_VERSION_S5L8940     "iBoot-838.8"
#define BOOTROM_VERSION_S5L8942     "ROM"
#define BOOTROM_VERSION_S5L8945     "iBoot-1062.2"
#define BOOTROM_VERSION_S5L8947     "ROM"
#define BOOTROM_VERSION_S5L8950     "iBoot-1145.3"
#define BOOTROM_VERSION_S5L8955     "iBoot-1145.3.3"

#define IPOD_TOUCH_NAME         "iPod touch"
#define IPAD_NAME               "iPad"
#define IPHONE_NAME             "iPhone"
#define APPLE_TV_NAME           "Apple TV"

struct device_description {
    uint32_t index;
    const char* product;
    const char* name;
    const char* colloqiual_name;
    uint32_t board_id;
    uint32_t chip_id;
};

typedef struct device_description device_description_t;

/*
 * Hyper-mega-super-duper device list.
 */
#ifdef _USE_DEVICE_LIST

static struct device_description device_list[] = {
    {   0, "iPhone1,1", "m68ap", IPHONE_NAME " 2G", 0x00, 0x8900},
    {   1, "iPhone1,2", "n82ap", IPHONE_NAME " 3G", 0x04, 0x8900},
    {   2, "iPhone2,1", "n88ap", IPHONE_NAME " 3GS", 0x00, 0x8920},
    {   3, "iPhone3,1", "n90ap", IPHONE_NAME " 4 GSM", 0x00, 0x8930},
    {   4, "iPhone3,2", "n90bap", IPHONE_NAME " 4 GSM (2012)", 0x04, 0x8930},
    {   5, "iPhone3,3", "n92ap", IPHONE_NAME " 4 CDMA", 0x06, 0x8930},
    {   6, "iPhone4,1", "n94ap", IPHONE_NAME " 4S", 0x08, 0x8940},
    {   7, "iPhone5,1", "n41ap", IPHONE_NAME " 5 (A1428)", 0x00, 0x8950},
    {   8, "iPhone5,2", "n42ap", IPHONE_NAME " 5 (A1429)", 0x02, 0x8950},
    {   9, "iPod1,1", "n45ap", IPOD_TOUCH_NAME " 1G", 0x02, 0x8900},
    {  10, "iPod2,1", "n72ap", IPOD_TOUCH_NAME " 2G", 0x00, 0x8720},
    {  11, "iPod3,1", "n18ap", IPOD_TOUCH_NAME " 3G", 0x02, 0x8922},
    {  12, "iPod4,1", "n81ap", IPOD_TOUCH_NAME " 4G", 0x08, 0x8930},
    {  13, "iPod5,1", "n78ap", IPOD_TOUCH_NAME " 5G", 0x00, 0x8942},
    {  14, "iPad1,1", "k48ap", IPAD_NAME " 1", 0x02, 0x8930},
    {  15, "iPad2,1", "k93ap", IPAD_NAME " 2 (Wi-Fi)", 0x04, 0x8940},
    {  16, "iPad2,2", "k94ap", IPAD_NAME " 2 (GSM)", 0x06, 0x8940},
    {  17, "iPad2,3", "k95ap", IPAD_NAME " 2 (CDMA)", 0x02, 0x8940},
    {  18, "iPad2,4", "k93aap", IPAD_NAME " 2 (Wi-Fi, revA)", 0x06, 0x8942},
    {  19, "iPad2,5", "p105ap", IPAD_NAME " mini (Wi-Fi)", 0x0a, 0x8942},
    {  20, "iPad2,6", "p106ap", IPAD_NAME " mini (GSM)", 0x0c, 0x8942},
    {  21, "iPad2,7", "p107ap", IPAD_NAME " mini (CDMA)", 0x0e, 0x8942},
    {  22, "iPad3,1", "j1ap", IPAD_NAME " 3 (Wi-Fi)", 0x00, 0x8945},
    {  23, "iPad3,2", "j2ap", IPAD_NAME " 3 (GSM)", 0x02, 0x8945},
    {  24, "iPad3,3", "j2aap", IPAD_NAME " 3 (CDMA)", 0x04, 0x8945},
    {  25, "iPad3,4", "p101ap", IPAD_NAME " 4 (Wi-Fi)", 0x00, 0x8955},
    {  26, "iPad3,5", "p102ap", IPAD_NAME " 4 (GSM)", 0x02, 0x8955},
    {  27, "iPad3,6", "p103ap", IPAD_NAME " 4 (CDMA)", 0x04, 0x8955},
    {  28, "AppleTV2,1", "k66ap", APPLE_TV_NAME " 2G", 0x10, 0x8930},
    {  29, "AppleTV3,1", "j33ap", APPLE_TV_NAME " 3G", 0x08, 0x8942},
    {  30, "AppleTV3,?", "j33iap", APPLE_TV_NAME " 3G (revA)", 0xFF, 0x8947},
    {  -1, NULL, NULL, NULL, -1, -1},
};
#endif

/*
 * API.
 */
extern struct device_description* device_probe_for_device __P((void));
extern uint32_t device_get_current_chip_id __P((void));
extern uint32_t device_get_current_board_id __P((void));
extern const char* device_get_current_product __P((void));
extern const char* device_get_current_name __P((void));
extern const char* device_get_localized_name __P((void));

#endif
