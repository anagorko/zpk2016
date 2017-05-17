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
    bool redraw = true;
    
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
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    //**********************************
    //game interface (queue loop)
    cpu game;
    string placer = "none";
    p last_mouse_position;
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_rest(0.5);
    al_start_timer(timer);

    int turn_counter = 0;

    while(run) { 
        ALLEGRO_EVENT ev; 
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || 
                ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
            last_mouse_position = p(ev.mouse.x, ev.mouse.y);
        } //no else
        
        if((placer != "none") && 
                (ev.type == ALLEGRO_EVENT_MOUSE_AXES || 
                 ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)) {
            game.set_placer(p(ev.mouse.x, ev.mouse.y));
        } else 
        if(ev.type == ALLEGRO_EVENT_TIMER){
            game.move_enemies();
            game.damage_enemies();
            redraw = true;
        } else
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else
        if(placer == "laser" && ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            game.add_laser(p(ev.mouse.x, ev.mouse.y));
            placer = "none";
            game.remove_placer();
        } else
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                break;
            } else
            if(ev.keyboard.keycode == ALLEGRO_KEY_R) {
                game.change_show_range_state();
            } else
            if(ev.keyboard.keycode == ALLEGRO_KEY_L) {
                placer = "laser";
                game.add_placer("laser");
                game.set_placer(last_mouse_position);
            } else
            if(ev.keyboard.keycode == ALLEGRO_KEY_Q) {
                placer = "none";
                game.remove_placer();
            }
        } 

        if(redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            if(turn_counter % 100 == 0) {
                game.add_basic_enemy();
            }
            if(turn_counter % 1000 == 0) {
                game.add_level();
            }

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
