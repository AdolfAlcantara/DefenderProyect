#include "screen.h"

//cada 2 bits una columna
//cada 160 bits una fila

//COLOR
//primeros 4 bits son para background
//segundos para foreground

uint8_t cursor_row = 0;
uint8_t cursor_column = 0;
uint8_t color_bg = 0;
uint8_t color_fg = 0;

void put_char(uint8_t ch){
    uint16_t data = 0x0000000000000000 | color_bg ;
    data = data << 4;
    data = data | color_fg;
    data = data << 8;
    data = data | ch;
    uint16_t *vgapptr = (uint16_t *)VGA_START_ADDR + (80*cursor_row)+cursor_column;

    *vgapptr = data;
}

void put_char_pos(uint8_t ch,uint16_t pos){
    uint16_t data = 0xfc00 | ch;
    uint16_t *vgapptr = (uint16_t *)VGA_START_ADDR+pos;

    *vgapptr = data;
}

void puts(char *word){
    uint8_t col;
    uint8_t row;
    get_cursor(&row,&col);
    while(*word){    
        put_char(*word);
        word++;
        set_cursor(row,++col);
    }
}

void clear_screen(){
    set_cursor(0,0);
    set_color(0,0);
    for(int i=0;i<30;i++){
        for(int j=0;j<80;j++){
            set_cursor(i,j);
            put_char(' ');
        }
    }
}

void set_cursor(uint8_t row, uint8_t column){
    if(column>79){
        row++;
        column=0;
    }
    if(row>29){
        row = 0;
        column = 0;
    }
    cursor_row = row;
    cursor_column = column;
}

void get_cursor(uint8_t *row, uint8_t *column){
    *row = cursor_row;
    *column = cursor_column;
}

void set_color( uint8_t bgcolor,uint8_t fgcolor){
    color_bg = bgcolor;
    color_fg = fgcolor;
}

void get_color(uint8_t *fgcolor, uint8_t *bgcolor){
    *bgcolor = color_bg;
    *fgcolor = color_fg;
}

int divq(int num1, int num2) 
{ 
   if (num1 == 0) 
       return 0; 
   if (num2 == 0) 
     return -1; 
  
   bool negResult = false; 
  
   // Handling negative numbers 
   if (num1 < 0) 
   { 
       num1 = -num1 ; 
       if (num2 < 0) 
           num2 = - num2 ;  
       else
           negResult = true; 
   } 
   else if (num2 < 0) 
   { 
       num2 = - num2 ; 
       negResult = true;  
   } 
  
   // if num1 is greater than equal to num2 
   // subtract num2 from num1 and increase 
   // quotient by one. 
   int quotient = 0; 
   while (num1 >= num2) 
   { 
       num1 = num1 - num2 ; 
       quotient++ ; 
   } 
  
   // checking if neg equals to 1 then making 
   // quotient negative  
   if (negResult) 
        quotient = - quotient ; 
   return quotient ; 
} 

int divr(int num1, int num2) 
{ 
   if (num1 == 0) 
       return 0; 
   if (num2 == 0) 
     return -1; 
  
   bool negResult = false; 
  
   // Handling negative numbers 
   if (num1 < 0) 
   { 
       num1 = -num1 ; 
       if (num2 < 0) 
           num2 = - num2 ;  
       else
           negResult = true; 
   } 
   else if (num2 < 0) 
   { 
       num2 = - num2 ; 
       negResult = true;  
   } 
  
   // if num1 is greater than equal to num2 
   // subtract num2 from num1 and increase 
   // quotient by one. 
   int quotient = 0; 
   while (num1 >= num2) 
   { 
       num1 = num1 - num2 ; 
       quotient++ ; 
   } 
  
   // checking if neg equals to 1 then making 
   // quotient negative  
   if (negResult) 
        quotient = - quotient ; 
   return num1 ; 
}

uint8_t get_rand(){
    uint8_t rnumber = *MS_COUNTER_REG_ADDR;
    rnumber = rnumber << 4;
    rnumber = rnumber >> 4;
    return rnumber;
}

// char* toStr(int n){
//     char *convertString = "0123456789ABCDEF";
//     int x = divr(n,10);
//     int y = divq(n,10);
//     // if (x < 10)
//       return convertString[x] + *convertString[y];
//     // else
//     //   return toStr(divq(n,10)) + convertString[divr(n,10)];
// }