#include "screen.c"
#include "keypad.c"

/*space shipt image location on .mif:  1-5
*bullet on .mif: 6-8
*enemy1 .mif:9-10
*enemy2 .mif:11-12
*enemy3 .mif:13-14
*bombs .mif:15-18
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

void detonate_bomb();
void create_bullet();

struct enemy enemies[10];
enemy enemy1,enemy2,enemy3;

spaceship spaceship_1;
bullet space_ship_bullet;
bool isShooting = false,dir=true;

uint8_t points;
uint8_t movement_count;
uint8_t lifes, bombs;

void initCharacters(){
    
    movement_count=0;
    points = 0;
    lifes = 3;
    bombs = 3;

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
    enemy2.pos_y = 20;
    enemy2.color_fg = 14;
    enemy2.fpart = 11;
    enemy2.spart = 12;

    enemy3.pos_x = 75;
    enemy3.pos_y = 13;
    enemy3.color_fg = 4;
    enemy3.fpart = 13;
    enemy3.spart = 14;


    enemies[0] = enemy1;
    enemies[1] = enemy2;
    enemies[2] = enemy3;

}

void spaceship_movement(uint8_t key){
    switch (key)
    {
        case 1:{
            spaceship_1.pos_x--;
            if(spaceship_1.pos_x<=0){
                spaceship_1.pos_x = 0;
            }
            break;
        }
        case 2:{
            spaceship_1.pos_x++;
            if(spaceship_1.pos_x>75){
                spaceship_1.pos_x = 75;
            }
            break;
        }
        case 4:{
            spaceship_1.pos_y++;
            if(spaceship_1.pos_y>=24){
                spaceship_1.pos_y = 24;
            }
            break;
        }
        case 8:{
            spaceship_1.pos_y--;
            if(spaceship_1.pos_y<0){
                spaceship_1.pos_y++;
            }
            break;
        }
        case 16:{
            if(bombs>0){
                detonate_bomb();
            }
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
}



void enemy_movement(){
    for (int i=0;i<10;i++){
        if(dir){
            enemies[i].pos_x += get_rand();
            if(enemies[i].pos_x >= 78){
                enemies[i].pos_x = 78;
            }
            enemies[i].pos_y += get_rand();
            if(enemies[i].pos_y >=24){
                enemies[i].pos_y = 24;
            }
        }else{
            enemies[i].pos_x -= get_rand();
            if(enemies[i].pos_x <= 0){
                enemies[i].pos_x = 0;
            }
            enemies[i].pos_y -= get_rand();
            if(enemies[i].pos_y <= 0){
                enemies[i].pos_y = 0;
            }
        }
    }
    dir=!dir;  
}

void create_bullet(){
    space_ship_bullet.pos_x = spaceship_1.pos_x+5;
    space_ship_bullet.pos_y = spaceship_1.pos_y;
    isShooting = true;
}

void check_collisions(){
    uint8_t bullet_tail = space_ship_bullet.pos_x;
    uint8_t bullet_head = bullet_tail + 3;
    uint8_t bullet_line = space_ship_bullet.pos_y;
    for(int i=0;i<10;i++){
        uint8_t e_line = enemies[i].pos_y;
        if(e_line == bullet_line){
            if(bullet_tail <= enemies[i].pos_x && enemies[i].pos_x <= bullet_head){
                set_cursor(0,0);
                put_char('5');
                for(int j=i;j<9;j++){
                    enemies[j] = enemies[j+1];
                }
                points++;
            }
        }
    }
}

void check_spaceship_collision(){
    uint8_t ss_pox = spaceship_1.pos_x;
    uint8_t ss_poy = spaceship_1.pos_y;
    for(int i=0;i<10;i++){
        if(ss_poy == enemies[i].pos_y){
            if(ss_pox <= enemies[i].pos_x && ss_pox+3 >= enemies[i].pos_x){
                spaceship_1.pos_x = 5;
                spaceship_1.pos_y = 17;
                lifes--;
            }
        }
    }
}

void detonate_bomb(){
    for(int i=0;i<10;i++){
       uint8_t e_pox = enemies[i].pos_x;
       uint8_t e_poy = enemies[i].pos_y;
        if(e_pox >= 0 && e_pox < 80 && 
           e_poy >= 0 && e_poy <= 25){
            for(int j=i;j<9;j++){
                enemies[j] = enemies[j+1];
            }
            points++;
        }
    }
    set_cursor(0,0);
    set_color(15,15);
    for(int i=0;i<25;i++){
        for(int j=0;j<80;j++){
            set_cursor(i,j);
            put_char(' ');
        }
    }
    bombs--;
}

void picture_points(){
    set_cursor(27,39);
    set_color(0,15);
    put_char(points);
}

void picture_lifes_gui(){
    uint8_t col = 2;
    set_color(14,1);
    for(int i=0;i<lifes;i++){
        for(int j=1;j<6;j++){
            set_cursor(27,col);
            put_char(j);
            col++;
        }
        col+=2;
    }
}

void picture_enemy(enemy e){
    set_color(0,e.color_fg);
    set_cursor(e.pos_y,e.pos_x);
    put_char(e.fpart);
    set_cursor(e.pos_y,e.pos_x+1);
    put_char(e.spart);
}


void picture_spaceship_bullet(){
    set_color(0,5);
    space_ship_bullet.pos_x += 5;
    if(space_ship_bullet.pos_x<77){
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

void picture_bombs(){
    uint8_t col = 52;
    set_color(14,1);
    for(int i=0;i<bombs;i++){
        for(int j=15;j<19;j++){
            set_cursor(27,col);
            put_char(j);
            col++;
        }
        col+=2;
    }
}

void render(){
    clear_screen();
    picture_spaceshipt();
    for(int i=0;i<10;i++){
        picture_enemy(enemies[i]);
    }
    if(isShooting){
        picture_spaceship_bullet();
        check_collisions();
    }
    picture_points();
    picture_lifes_gui();
    picture_bombs();
}