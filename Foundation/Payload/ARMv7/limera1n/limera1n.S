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

/*
 * limera1n exploit module.
 *
 * Either check to see if the image is an Img3 file or not. Directly transfer
 * control to the image if it isn't one. If it is, just decrypt and jump.
 *
 * No patching like the greenpois0n payload. That's done in software entirely.
 */

#include <Payload/Offsets.h>
#include <Assembler.h>

.text
.code 16

.globl _start

_start:
    mov     r7, pc
    b       payload_entry
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

payload_entry:
    /*
     * Get version.
     */
    ldr     r0, =BOOTROM_VERSION_ADDRESS
    ldr     r2, [r0]
    
    ldr     r0, =BOOTROM_VERSION_S5L8920
    cmp     r0, r2
    beq     _detected_s5l8920

    ldr     r0, =BOOTROM_VERSION_S5L8920_NEW
    cmp     r0, r2
    beq     _detected_s5l8920_new

    ldr     r0, =BOOTROM_VERSION_S5L8922
    cmp     r0, r2
    beq     _detected_s5l8922

    ldr     r0, =BOOTROM_VERSION_S5L8930
    cmp     r0, r2
    beq     _detected_s5l8930

    /*
     * If all else fails, fall into the Inferno.
     */ 
    b       .

_detected_s5l8920:
    ldr     r1, =__iBoot_JumpTo_S5L8920
    str     r1, [r7]
    ldr     r1, =__iBoot_UsbWaitForImage_S5L8920
    str     r1, [r7, #4]
    ldr     r1, =__iBoot_AesCryptoCmd_S5L8920
    str     r1, [r7, #8]
    b       _wait_for_image

_detected_s5l8920_new:
    ldr     r1, =__iBoot_JumpTo_S5L8920_NEW
    str     r1, [r7]
    ldr     r1, =__iBoot_UsbWaitForImage_S5L8920_NEW
    str     r1, [r7, #4]
    ldr     r1, =__iBoot_AesCryptoCmd_S5L8920_NEW
    str     r1, [r7, #8]
    b       _wait_for_image

_detected_s5l8922:
    ldr     r1, =__iBoot_JumpTo_S5L8922
    str     r1, [r7]
    ldr     r1, =__iBoot_UsbWaitForImage_S5L8922
    str     r1, [r7, #4]
    ldr     r1, =__iBoot_AesCryptoCmd_S5L8922
    str     r1, [r7, #8]
    b       _wait_for_image

_detected_s5l8930:
    ldr     r1, =__iBoot_JumpTo_S5L8930
    str     r1, [r7]
    ldr     r1, =__iBoot_UsbWaitForImage_S5L8930
    str     r1, [r7, #4]
    ldr     r1, =__iBoot_AesCryptoCmd_S5L8930
    str     r1, [r7, #8]
    b       _wait_for_image

    /*
     * Unpack the image.
     */

_wait_for_image:
    ldr     r0, =LOAD_ADDRESS
    ldr     r2, [r0]
    ldr     r0, =TAG_IMG3
    cmp     r0, r2
    bne     _jump_directly_to_image

_decrypt_image:
    ldr     r0, =TAG_DATA
    ldr     r1, =LOAD_ADDRESS
    ldr     r2, [r1, #4]
    bl      find
    cmp     r0, #0
    beq     payload_entry
    mov     r6, r0

    ldr     r0, =TAG_DATA
    ldr     r1, =LOAD_ADDRESS
    ldr     r2, [r1, #4]
    bl      find
    cmp     r0, #0
    beq     payload_entry
    mov     r5, r0

    mov     r0, #0x11
    mov     r4, #0x14
    add     r1, r5, r4
    add     r2, r5, r4
    mov     r3, #0x30
    ldr     r4, =AES_TYPE_GID
    str     r4, [sp]
    mov     r4, #0
    str     r4, [sp, #4]
    mov     r4, #0
    str     r4, [sp, #8]
    ldr     r4, [r7, #4]
    blx     r4

    mov     r0, #0x11
    mov     r4, #0xC
    add     r1, r6, r4
    add     r2, r6, r4
    ldr     r3, [r6, #0x8]
    ldr     r4, =AES_MODE_256
    str     r4, [sp]
    mov     r4, #0x24
    add     r4, r5
    str     r4, [sp, #4]
    mov     r4, #0x14
    add     r4, r5
    str     r4, [sp, #8]
    ldr     r4, [r7, #4]
    blx     r4

    /*
     * Move image data to load address.
     */
    ldr     r0, =LOAD_ADDRESS
    mov     r1, #0xC
    add     r1, r6
    ldr     r2, [r6, #8]
    mov     r5, r2
    bl      memcpy

_jump_directly_to_image:
    mov     r0, #0
    ldr     r1, =LOAD_ADDRESS
    mov     r2, #0
    ldr     r3, [r7, #8]
    blx     r3
    b       .

/*
 * Support functionality.
 */
memcpy:
_memcpy_loop:
    ldrb    r3, [r1]
    strb    r3, [r0]
    add     r0, #1
    add     r1, #1
    sub     r2, #1
    cmp     r2, #0
    bne     _memcpy_loop
    bx      lr

find:
    mov     r3, #0
    b       _find64
_find64:
    push    {r4,lr}
_find_loop:
    ldr     r4, [r1]
    cmp     r4, r0
    bne     _find_loop_continue
    cmp     r3, #0
    beq     _find_ret
    ldr     r4, [r1, #4]
    cmp     r4, r3
    beq     _find_ret
_find_loop_continue:
    add     r1, #2
    sub     r2, #2
    cmp     r2, #0
    bne     _find_loop
    mov     r1, #0
_find_ret:
    mov     r0, r1
    pop     {r4,pc}


