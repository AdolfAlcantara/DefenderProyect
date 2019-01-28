#ifndef _VGA_H_
#define _VGA_H_

#include "system.h"

void clear_screen();
void set_cursor(uint8_t row, uint8_t column);
void get_cursor(uint8_t *row, uint8_t *column);
void set_color(uint8_t fgcolor, uint8_t bgcolor);
void get_color(uint8_t *fgcolor, uint8_t *bgcolor);
void put_char(uint8_t ch);
void puts(char *str);
void put_decimal(uint32_t n);

#endif
