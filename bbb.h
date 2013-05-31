/*
 * avrdude - A Downloader/Uploader for AVR device programmers
 * Copyright (C) 2000, 2001, 2002, 2003  Brian S. Dean <bsd@bsdhome.com>
 * Copyright (C) 2005 Michael Holzt <kju-avr@fqdn.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* $Id: bbb.h 722 2007-01-24 22:43:46Z joerg_wunsch $ */

#ifndef bbb_h
#define bbb_h

#ifdef __cplusplus
extern "C" {
#endif


#define BBB_GPIO_PIN_DEFS(ENTRY) \
    /* GPIO#, HDR, Pin */ \
    ENTRY(30,  P9,  11), \
    ENTRY(60,  P9,  12), \
    ENTRY(31,  P9,  13), \
    ENTRY(40,  P9,  14), \
    ENTRY(48,  P9,  15), \
    ENTRY(51,  P9,  16), \
    ENTRY(4,   P9,  17), \
    ENTRY(5,   P9,  18), \
    ENTRY(3,   P9,  21), \
    ENTRY(2,   P9,  22), \
    ENTRY(49,  P9,  23), \
    ENTRY(15,  P9,  24), \
    ENTRY(117, P9,  25), \
    ENTRY(14,  P9,  26), \
    ENTRY(125, P9,  27), \
    ENTRY(123, P9,  28), \
    ENTRY(121, P9,  29), \
    ENTRY(122, P9,  30), \
    ENTRY(120, P9,  31), \
    ENTRY(20,  P9,  41), \
    ENTRY(7,   P9,  42), \
    ENTRY(38,  P8,  3), \
    ENTRY(39,  P8,  4), \
    ENTRY(34,  P8,  5), \
    ENTRY(35,  P8,  6), \
    ENTRY(66,  P8,  7), \
    ENTRY(67,  P8,  8), \
    ENTRY(69,  P8,  9), \
    ENTRY(68,  P8,  10), \
    ENTRY(45,  P8,  11), \
    ENTRY(44,  P8,  12), \
    ENTRY(23,  P8,  13), \
    ENTRY(26,  P8,  14), \
    ENTRY(47,  P8,  15), \
    ENTRY(46,  P8,  16), \
    ENTRY(27,  P8,  17), \
    ENTRY(65,  P8,  18), \
    ENTRY(22,  P8,  19), \
    ENTRY(63,  P8,  20), \
    ENTRY(62,  P8,  21), \
    ENTRY(37,  P8,  22), \
    ENTRY(36,  P8,  23), \
    ENTRY(33,  P8,  24), \
    ENTRY(32,  P8,  25), \
    ENTRY(61,  P8,  26), \
    ENTRY(86,  P8,  27), \
    ENTRY(88,  P8,  28), \
    ENTRY(87,  P8,  29), \
    ENTRY(89,  P8,  30), \
    ENTRY(10,  P8,  31), \
    ENTRY(11,  P8,  32), \
    ENTRY(9,   P8,  33), \
    ENTRY(81,  P8,  34), \
    ENTRY(8,   P8,  35), \
    ENTRY(80,  P8,  36), \
    ENTRY(78,  P8,  37), \
    ENTRY(79,  P8,  38), \
    ENTRY(76,  P8,  39), \
    ENTRY(77,  P8,  40), \
    ENTRY(74,  P8,  41), \
    ENTRY(75,  P8,  42), \
    ENTRY(72,  P8,  43), \
    ENTRY(73,  P8,  44), \
    ENTRY(70,  P8,  45), \
    ENTRY(71,  P8,  46)



void bbb_initpgm        (PROGRAMMER * pgm);

#ifdef __cplusplus
}
#endif

#endif
