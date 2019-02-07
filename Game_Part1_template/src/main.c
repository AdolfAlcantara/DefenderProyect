#include "screen.c"
#include "keypad.c"

/*space shipt image location on .mif:  1-5
*bullet on .mif: 6-8
*enemy1 .mif:9-10
*enemy2 .mif:11-12
*enemy3 .mif:13-14
*/ 
typedef struct spaceship
{
    uint8_t pos_x;
    uint8_t pos_y;
} spaceship;

typedef struct bullet
{
    uint8_t pos_x;
    uint8_t pos_y;
} bullet;

typedef struct enemy{
    uint8_t pos_x;
    uint8_t pos_y;
    uint8_t color_fg;
    int fpart,spart;
}enemy;

struct enemy enemies[10];

enemy enemy1,enemy2,enemy3;
spaceship spaceship_1;
bullet space_ship_bullet;
bool isShooting = false,dir=true;
uint8_t movement_count=0;

void initCharacters(){
    
    movement_count=0;

    //space_ship
    spaceship_1.pos_x = 5;
    spaceship_1.pos_y = 14;

    //space_ship_bullet
    space_ship_bullet.pos_x = 0;
    space_ship_bullet.pos_y = 0;

    //enemies
    enemy1.pos_x=50;
    enemy1.pos_y=6;
    enemy1.color_fg=10;
    enemy1.fpart=9;
    enemy1.spart=10;

    enemy2.pos_x=30;
    enemy2.pos_y = 23;
    enemy2.color_fg = 14;
    enemy2.fpart = 11;
    enemy2.spart = 12;

    enemy3.pos_x = 75;
    enemy3.pos_y = 13;
    enemy3.color_fg = 4;
    enemy3.fpart = 13;
    enemy3.spart = 14;

    // for(int i=0;i<10;i++){
    //     enemies[i] = null;
    // }

    enemies[0] = enemy1;
    enemies[1] = enemy2;
    enemies[2] = enemy3;

}

void enemy_movement(){
    for (int i=0;i<10;i++){
        if(dir){
            enemies[i].pos_x += get_rand();
            enemies[i].pos_y += get_rand();
        }else{
            enemies[i].pos_x -= get_rand();
            enemies[i].pos_y -= get_rand();
        }
    }
    dir=!dir;  
}

void picture_enemy(enemy e){
    set_color(0,e.color_fg);
    set_cursor(e.pos_y,e.pos_x);
    put_char(e.fpart);
    set_cursor(e.pos_y,e.pos_x+1);
    put_char(e.spart);
}

void create_bullet(spaceship ss){
    space_ship_bullet.pos_x = ss.pos_x+5;
    space_ship_bullet.pos_y = ss.pos_y;
    isShooting = true;
}

void picture_spaceship_bullet(){
    set_color(0,5);
    if(space_ship_bullet.pos_x<75){
        space_ship_bullet.pos_x += 5;
        uint8_t col = space_ship_bullet.pos_x;
        set_cursor(space_ship_bullet.pos_y,space_ship_bullet.pos_x);
        for(uint8_t i=6; i<9; i++){
            put_char(i);
            set_cursor(space_ship_bullet.pos_y,--col);
        }
        return;
    }
    isShooting = false;
}

void picture_spaceshipt(){
    set_color(0,11);
    uint8_t column = spaceship_1.pos_x;
    uint8_t row = spaceship_1.pos_y;
    for(uint8_t i=1; i<6; i++){
        set_cursor(row,column++);
        put_char(i);
    }
}

void render(){
    clear_screen();
    picture_spaceshipt();
    for(int i=0;i<10;i++){
        picture_enemy(enemies[i]);
    }
    // picture_enemy(enemy1);
    // picture_enemy(enemy2);
    // picture_enemy(enemy3);
    if(isShooting){
        picture_spaceship_bullet();
    }
}

int main(){
;
    // uint8_t rowc;
    // uint8_t columnc;
    // set_cursor(row,column);
    // get_cursor(&rowc,&columnc);
    // uint16_t pos = rowc;
    clear_screen();
    set_color(0,0);
    set_cursor(0,0);
    // put_char('A');
    // put_char_pos('A',40);
    // puts(toStr(11));
    // put_char(' ');

    initCharacters();
    picture_spaceshipt();
    uint8_t key;
    while(true){
        key = keypad_getkey();
        switch (key)
        {
            case 1:{
                spaceship_1.pos_x--;
                break;
            }
            case 2:{
                spaceship_1.pos_x++;
                break;
            }
            case 4:{
                spaceship_1.pos_y++;;
                break;
            }
            case 8:{
                spaceship_1.pos_y--;;
                break;
            }
            case 128:{
                create_bullet(spaceship_1);
                break;
            }
            default:{
                break;
            }
        }
        if(movement_count == 12){   
            enemy_movement();
            movement_count=0;
        }else{
            movement_count++;
        }
        delay_ms(100);
        render();
        
    }

return 0;
}