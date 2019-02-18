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

void clear_screen_endgame(){
    set_cursor(0,0);
    set_color(15,0);
    for(int i=0;i<30;i++){
        for(int j=0;j<80;j++){
            set_cursor(i,j);
            put_char(' ');
        }
    }
}

void clear_screen(){
    set_cursor(0,0);
    set_color(0,0);
    for(int i=0;i<25;i++){
        for(int j=0;j<80;j++){
            set_cursor(i,j);
            put_char(' ');
        }
    }
    for(int i=25;i<30;i++){
        for(int j=0;j<80;j++){
            set_color(14,0);
            if(j>=29 && j<=49 && i>=26 && i<=28){
                set_color(0,0);
            }
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

int get_rand(){
    uint8_t rnumber = *MS_COUNTER_REG_ADDR;
    rnumber = rnumber << 5;
    rnumber = rnumber >> 5;
    return rnumber;
}

// char* convertInt(int number)
// {
//     if (number == 0)
//         return "0";
//     char* temp="";
//     char* returnvalue="";
//     while (number>0)
//     {
//         temp+=(divr(number,10)+48);
//         number = divq(number,0);
//     }
//     for (int i=0;i<temp.length();i++)
//         returnvalue+=temp[temp.length()-i-1];
//     return returnvalue;
// }