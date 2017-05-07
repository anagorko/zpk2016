#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "cpu.h"
#include "enemy.h"
#include "turret.h"
#include "effect.h"



int main(int argc, char** argv) {
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    ALLEGRO_KEYBOARD_STATE klawiatura;
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    ALLEGRO_DISPLAY* okno = al_create_display(800, 600);
    al_set_window_title(okno, "Pfffffffff!");

    //**********************************
    //tu piszemy faktyczny program
    cpu game;
    game.add_laser(p(450,360));
    game.add_laser(p(510,400));
    game.add_laser(p(300,250));
    game.add_basic_enemy();

    for(int i = 0; i < 20; i++) {
        for(int i = 0; i < 100; i++) {
            game.move_enemies();
            game.damage_enemies();
            game.display_all();
            al_rest(0.01);
        }
        game.add_basic_enemy();
    }
    for(int i = 0; i < 500; i++) {
        game.move_enemies();
        game.damage_enemies();
        game.display_all();
        al_rest(0.01);
    }
    
    //**********************************
    al_destroy_display(okno);

    return 0;
}
