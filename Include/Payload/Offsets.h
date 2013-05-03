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

#ifndef _OFFSETS_H_
#define _OFFSETS_H_

#define BOOTROM_VERSION_S5L8920             0x00332e39
#define BOOTROM_VERSION_S5L8920_NEW         0xe2333239
#define BOOTROM_VERSION_S5L8922             0x0035e229
#define BOOTROM_VERSION_S5L8930             0x0034e234

#define __iBoot_JumpTo_S5L8920              0x3969
#define __iBoot_UsbWaitForImage_S5L8920     0x349D
#define __iBoot_AesCryptoCmd_S5L8920        0x925

#define __iBoot_JumpTo_S5L8920_NEW          0x3971
#define __iBoot_UsbWaitForImage_S5L8920_NEW 0x349D
#define __iBoot_AesCryptoCmd_S5L8920_NEW    0x925

#define __iBoot_JumpTo_S5L8922              0x39DD
#define __iBoot_UsbWaitForImage_S5L8922     0x36E5
#define __iBoot_AesCryptoCmd_S5L8922        0x919

#define __iBoot_JumpTo_S5L8930              0x5A5D
#define __iBoot_UsbWaitForImage_S5L8930     0x4C85
#define __iBoot_AesCryptoCmd_S5L8930        0x686D

#define LOAD_ADDRESS                    0x84000000
#define MAX_SIZE                        0x2C000
#define BOOTROM_VERSION_ADDRESS         0x288

#define TAG_IMG3            0x496D6733
#define TAG_DATA            0x44415441
#define TAG_KBAG            0x4B424147

#define AES_TYPE_GID        0x20000200
#define AES_MODE_256        0x20000000

#endif
