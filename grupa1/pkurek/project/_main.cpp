#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "cpu.h"
#include "enemy.h"
#include "turret.h"
#include "effect.h"
#include <stdio.h>


const float FPS = 60;

int main(int argc, char** argv) {
    int run = 1;
    bool make_turn = true;
    
    //inits
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();


    //devices
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    al_set_window_title(display, "Pfffffffff!");
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if(!(display && event_queue && timer)) {run = 0;}

    al_install_mouse();
    al_install_keyboard();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    
    //**********************************
    //game interface (queue loop)
    cpu game;
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_rest(0.5);
    al_start_timer(timer);

    int turn_counter = 0;

    while(run) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_TIMER){
            make_turn = true;
        } else
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else
        if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            game.add_laser(p(ev.mouse.x, ev.mouse.y));
        }

        if(make_turn && al_is_event_queue_empty(event_queue)) {
            make_turn = false;

            if(turn_counter % 100 == 0) {
                game.add_basic_enemy();
            }
            if(turn_counter % 1000 == 0) {
                game.add_level();
            }

            game.move_enemies();
            game.damage_enemies();
            game.display_all();
            turn_counter ++;

            if(game.get_lives() <= 0 ) {al_rest(2);break;}
        }
        
    }

    
    //**********************************
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
