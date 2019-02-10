
#include "game.c"



int main(){

    clear_screen();
    set_color(0,0);
    set_cursor(0,0);


    initCharacters();
    picture_spaceshipt();
    while(true){
        spaceship_movement(keypad_getkey());

        if(movement_count == 12){   
            enemy_movement();
            movement_count=0;
        }else{
            movement_count++;
        }
        check_spaceship_collision();
        delay_ms(100);
        render();
        
    }

return 0;
}