/*
 * avrdude - A Downloader/Uploader for AVR device programmers
 * Copyright (C) 2000, 2001, 2002, 2003  Brian S. Dean <bsd@bsdhome.com>
 * Copyright (C) 2005 Michael Holzt <kju-avr@fqdn.org>
 * Copyright (C) 2006 Joerg Wunsch <j@uriah.heep.sax.de>
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
/* $Id: bbb.c 976 2011-08-23 21:03:36Z joerg_wunsch $ */

/*
 * Posix serial bitbanging interface for avrdude.
 */

#if !defined(WIN32NATIVE)

#include "ac_cfg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "avr.h"
#include "pindefs.h"
#include "pgm.h"
#include "bitbang.h"
#include "bbb.h"



/* --------------- Macros --------------- */

//#define DEBUG
#undef DEBUG
#define EXPAND_AS_PIN_DATA(gpio_num, header, pin_num) \
    [ gpio_num ] = { gpio_num, #header "_" #pin_num,                    \
                     "/sys/class/gpio/gpio" #gpio_num "/value",         \
                     "/sys/class/gpio/gpio" #gpio_num "/direction",     \
                     -1, -1 }



/* --------------- Types --------------- */

typedef struct pin_data_t pin_data_t;
struct pin_data_t {
    int num;
    char* name;
    char* value_file;
    char* direction_file;
    int value_fd;
    int direction_fd;
};



/* --------------- Global Data --------------- */

static pin_data_t pins[] = {
    BBB_GPIO_PIN_DEFS(EXPAND_AS_PIN_DATA),
};
static const int num_pins = sizeof(pins) / sizeof(pin_data_t);



/* --------------- Helpers --------------- */

static pin_data_t* get_pin_data(int pin)
{
    pin_data_t* pdata;

    if (pin > num_pins) {
        return NULL;
    }

    pdata = &pins[pin];
    if (pdata->name == NULL) {
        return NULL;
    }

    return pdata;
}

static int bbb_gpio_open(pin_data_t* pin)
{
    if (pin->value_fd < 0) {
        pin->value_fd = open(pin->value_file, O_RDWR);
    }

    return (pin->value_fd >= 0 ? 0 : -1);
}

static void bbb_gpio_close(pin_data_t* pin)
{
    if (pin->value_fd >= 0) {
        close(pin->value_fd);
        pin->value_fd = -1;
    }
}

static int bbb_gpio_set(pin_data_t* pin, int value)
{
    char buffer[16];
    int bytes_written;

    bbb_gpio_open(pin);

    value = value ? 1 : 0;

    sprintf(buffer, "%d", value);
    bytes_written = write(pin->value_fd, buffer, strlen(buffer));
    bbb_gpio_close(pin);
    if (bytes_written < 1) {
        return 0;
    }
    return 0;
}

static int bbb_gpio_get(pin_data_t* pin)
{
    int bytes_read;
    char buffer[16] = { 0, };

    bbb_gpio_open(pin);

    bytes_read = read(pin->value_fd, &buffer, sizeof(buffer));
    if (bytes_read < 0) {
        return -1;
    }
    bbb_gpio_close(pin);

    if (buffer[0] == '1') {
        return 1;
    } else {
        return 0;
    }
}

static int bbb_setpin(PROGRAMMER * pgm, int pin, int value)
{
    pin_data_t* pin_data;

    if (pin & PIN_INVERSE) {
        value = !value;
        pin &= PIN_MASK;
    }

    pin_data = get_pin_data(pin);
    if (pin_data == NULL) {
        return -1;
    }

#ifdef DEBUG
    printf("%s:  pin %d <= %s (%s)\n", __func__, pin_data->num, value ? "set" : "clr", pin_data->name);
#endif

    bbb_gpio_set(pin_data, value);

    if (pgm->ispdelay > 1)
        bitbang_delay(pgm->ispdelay);

    return 0;
}

static int bbb_getpin(PROGRAMMER * pgm, int pin)
{
    int value;
    unsigned char invert = 0;
    pin_data_t* pin_data;

    if (pin & PIN_INVERSE) {
        invert = 1;
        pin   &= PIN_MASK;
    }

    pin_data = get_pin_data(pin);
    if (pin_data == NULL) {
        return -1;
    }

    value = bbb_gpio_get(pin_data);

    if (invert) {
        value = !value;
    }

#ifdef DEBUG
    printf("%s:  pin %d = %s (%s)\n", __func__, pin_data->num, value ? "set" : "clr", pin_data->name);
#endif

    return value;
}

static int bbb_highpulsepin(PROGRAMMER * pgm, int pin)
{
    bbb_setpin(pgm, pin, 1);
    bbb_setpin(pgm, pin, 0);

    return 0;
}



#define PIN_SYMBOL(x) [x] = { x, #x }

static void bbb_display(PROGRAMMER *pgm, const char *p)
{
    int pin;
    struct pin_symbol {
        int num;
        char* name;
    } pin_syms[N_PINS] = {
        PIN_SYMBOL(PPI_AVR_VCC),
        PIN_SYMBOL(PPI_AVR_BUFF),
        PIN_SYMBOL(PIN_AVR_RESET),
        PIN_SYMBOL(PIN_AVR_SCK),
        PIN_SYMBOL(PIN_AVR_MOSI),
        PIN_SYMBOL(PIN_AVR_MISO),
        PIN_SYMBOL(PIN_LED_ERR),
        PIN_SYMBOL(PIN_LED_RDY),
        PIN_SYMBOL(PIN_LED_PGM),
        PIN_SYMBOL(PIN_LED_VFY)
    };

    printf("\n\tBBB Pin Connections:\n");
    for (pin = 0; pin < N_PINS; pin++) {
        int pin_num = (pgm->pinno[pin] & PIN_MASK);
        pin_data_t* pdata;

        pdata = get_pin_data(pin_num);
        if (pdata) {
            printf("\t%s => %s\n", pin_syms[pin].name, pdata->name);
        }
    }
}

static void bbb_enable(PROGRAMMER *pgm)
{
}

static void bbb_disable(PROGRAMMER *pgm)
{
}

static void bbb_powerup(PROGRAMMER *pgm)
{
}

static void bbb_powerdown(PROGRAMMER *pgm)
{
}

static int bbb_open(PROGRAMMER *pgm, char *port)
{
  bitbang_check_prerequisites(pgm);

  return(0);
}

static void bbb_close(PROGRAMMER *pgm)
{
}

void bbb_initpgm(PROGRAMMER *pgm)
{
  strcpy(pgm->type, "BBB");

  pgm->rdy_led        = bitbang_rdy_led;
  pgm->err_led        = bitbang_err_led;
  pgm->pgm_led        = bitbang_pgm_led;
  pgm->vfy_led        = bitbang_vfy_led;
  pgm->initialize     = bitbang_initialize;
  pgm->display        = bbb_display;
  pgm->enable         = bbb_enable;
  pgm->disable        = bbb_disable;
  pgm->powerup        = bbb_powerup;
  pgm->powerdown      = bbb_powerdown;
  pgm->program_enable = bitbang_program_enable;
  pgm->chip_erase     = bitbang_chip_erase;
  pgm->cmd            = bitbang_cmd;
  pgm->cmd_tpi        = bitbang_cmd_tpi;
  pgm->open           = bbb_open;
  pgm->close          = bbb_close;
  pgm->setpin         = bbb_setpin;
  pgm->getpin         = bbb_getpin;
  pgm->highpulsepin   = bbb_highpulsepin;
  pgm->read_byte      = avr_read_byte_default;
  pgm->write_byte     = avr_write_byte_default;
}

#endif  /* WIN32NATIVE */
