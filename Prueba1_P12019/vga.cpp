#include "system.h"
#include "vga.h"

uint16_t vgafb[VGAFB_SIZE]; // VGA frame buffer

uint8_t cursor_col=0;
uint8_t cursor_row= 0;
uint8_t bg_color=0;
uint8_t fg_color=0;


/*
  TODO implement the following functions:
  clear_screen
  set_cursor
  set_color
  get_cursor
  put_char
*/

void clear_screen() {
    // set_cursor(0,0);
    // set_color(15,15);
    for(int i=0;i<ROW_COUNT;i++){
      for(int j=0;j<COL_COUNT;j++){
        set_cursor(i,j);
        put_char(' ');
      }
    }
}

void set_cursor(uint8_t row, uint8_t column) {
  cursor_row = row;
  cursor_col = column;
}

void get_cursor(uint8_t *row, uint8_t *column) {
  *row = cursor_row;
  *column = cursor_col;
}

void set_color(uint8_t fgcolor, uint8_t bgcolor) {
  bg_color = bgcolor;
  fg_color = fgcolor;
}

void put_char(uint8_t ch) {
  uint16_t data = 0x0000 | bg_color;
  data = data << 4;
  data = data | fg_color;
  data = data << 8;
  data = data | ch;
  set_cursor(cursor_row,cursor_col+1);
  if(ch == '\n'){
    set_cursor(cursor_row+1,0);
  }
  uint16_t *vgafb = (uint16_t *)(80*cursor_row)+cursor_col;
}