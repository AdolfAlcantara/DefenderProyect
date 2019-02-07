#include "system.h"

void put_char(uint8_t ch);
// void put_char2(uint8_t ch, int pos, char value);
void puts(char *);
void put_char_pos(uint8_t ch,uint16_t pos);

void clear_screen();
void set_cursor(uint8_t row, uint8_t column);
void get_cursor(uint8_t *row, uint8_t *column);
void set_color(uint8_t fgcolor, uint8_t bgcolor);
void get_color(uint8_t *fgcolor, uint8_t *bgcolor);
// void put_decimal(uint32_t n);
int division(int num1, int num2);
