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
    uint8_t type;
    int fpart,spart;
}enemy;

void detonate_bomb();
void create_bullet();

struct enemy enemies[10];
struct enemy enemy1,enemy2,enemy3;

spaceship spaceship_1;
bullet space_ship_bullet;
bool isShooting = false,dir=true;

uint8_t points;
uint8_t movement_count;
uint8_t lifes, bombs,enemy_count;

void initCharacters(){
    
    movement_count=0;
    points = 0;
    lifes = 3;
    bombs = 3;
    enemy_count = 3;

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
    enemy1.type = 1;

    enemy2.pos_x=30;
    enemy2.pos_y = 20;
    enemy2.color_fg = 14;
    enemy2.fpart = 11;
    enemy2.spart = 12;
    enemy2.type = 2;

    enemy3.pos_x = 75;
    enemy3.pos_y = 13;
    enemy3.color_fg = 4;
    enemy3.fpart = 13;
    enemy3.spart = 14;
    enemy3.type = 3;


    enemies[0] = enemy1;
    enemies[1] = enemy2;
    enemies[2] = enemy3;

}

void spaceship_movement(uint8_t key){
    switch (key)
    {
        case 1:{
            // spaceship_1.pos_x-=2;
            if(spaceship_1.pos_x>=2){
                spaceship_1.pos_x -=2;
            }
            break;
        }
        case 2:{
            spaceship_1.pos_x+=2;
            if(spaceship_1.pos_x>75){
                spaceship_1.pos_x = 75;
            }
            break;
        }
        case 3:{
            spaceship_1.pos_y++;
            if(spaceship_1.pos_y>=24){
                spaceship_1.pos_y = 24;
            }
            break;
        }
        case 4:{
            // spaceship_1.pos_y--;
            if(spaceship_1.pos_y>1){
                spaceship_1.pos_y--;
            }
            break;
        }
        case 5:{
            if(bombs>0){
                detonate_bomb();
            }
            break;
        }
        case 8:{
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
        if(enemies[i].pos_x==80 && enemies[i].pos_y ==30){
            continue;
        }
        int rand_numb = get_rand();
        // switch(enemies[i].type){
            // case 1:{
        if(enemies[i].type==1){
            if(dir){
                enemies[i].pos_x-=get_rand();
                enemies[i].pos_y+=get_rand();    
            }else{
                enemies[i].pos_x+=get_rand();
                enemies[i].pos_y-=get_rand();
            }
        }
        if(enemies[i].type==2){
            if(!dir){
                enemies[i].pos_x-=get_rand();        
            }else{
                enemies[i].pos_x+=get_rand();
            }
            enemies[i].pos_y+=MS_COUNTER_REG_ADDR[0];
        }
        if(enemies[i].type==3){
            if(enemies[i].pos_x>spaceship_1.pos_x){
                enemies[i].pos_x-=5;
            }else{
                enemies[i].pos_x+=5;
            }
            if(enemies[i].pos_y>spaceship_1.pos_y){
                enemies[i].pos_y-=2;
            }else{
                enemies[i].pos_y+=2;
            }
        }
        if(enemies[i].pos_x > 78){
            enemies[i].pos_x = 78;
        }
        if(enemies[i].pos_y >= 24){
            enemies[i].pos_y = 24;
        }

        
        // if(dir){
        //     enemies[i].pos_x += get_rand();
        //     
        //     enemies[i].pos_y += get_rand();
        //     if(enemies[i].pos_y >=24){
        //         enemies[i].pos_y = 24;
        //     }
        // }else{
        //     enemies[i].pos_x -= get_rand();
        //     if(enemies[i].pos_x <= 0){
        //         enemies[i].pos_x = 0;
        //     }
        //     enemies[i].pos_y -= get_rand();
        //     if(enemies[i].pos_y <= 0){
        //         enemies[i].pos_y = 0;
        //     }
        // }
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
                enemies[i].pos_x = 80;
                enemies[i].pos_y = 30;
                enemy_count--;
                points+=150;
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
           e_poy >= 0 && e_poy <= 26){
            enemies[i].pos_x = 80;
            enemies[i].pos_y = 30;
            points+=150;
            enemy_count--;
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
    uint8_t col = 40;
    set_color(0,15);
    int y = divq(points,10);
    int x = divr(points,10);
    while(y>10){
        set_cursor(26,col);
        put_char(x+48);
        x = divr(y,10);
        y = divq(y,10);
        col--;
    }
    if(x!=0){
        set_cursor(26,col);
        put_char(x+48);
        col--;
        if(y!=0){
            set_cursor(26,col);
            put_char(y+48);
        }
    }
    
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
    if(e.pos_x==80 && e.pos_y==30){
        return;
    }
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

void printEndMessage(){
    
    if(lifes ==0){
        puts("END GAME, YOU LOOSE");
    }else{
        puts("END GAME, YOU WON");
    }
}

void render(){
    if(lifes == 0 || enemy_count == 0){
        clear_screen_endgame();
        set_cursor(15,30);
        printEndMessage();
    }else{
        clear_screen();
        picture_spaceshipt();
        set_cursor(0,0);
        // puts(convertInt(enemy_count));
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
}