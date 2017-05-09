#include "drawer.h"
#include "cpu.h"
drawer::drawer(){}

void drawer::dr_background() {
    al_clear_to_color(al_map_rgba(bg_color.r, bg_color.g, bg_color.b, bg_color.a));
}

void drawer::set_window_size(p _window_size) {
    window_size = _window_size;
}

void drawer::set_board(vector<p>& _board) {
    board = _board;
}

void drawer::dr_border() {
    al_draw_filled_rectangle(0,0,window_size.x, board[1].y,
                             al_map_rgba(border_color.r, border_color.g,
                                         border_color.b, border_color.a));
    al_draw_filled_rectangle(0,0,board[3].x, window_size.y, 
                             al_map_rgba(border_color.r, border_color.g,
                                         border_color.b, border_color.a));
    al_draw_filled_rectangle(0,board[3].y,window_size.x, window_size.y,
                             al_map_rgba(border_color.r, border_color.g, 
                                         border_color.b, border_color.a));
    al_draw_filled_rectangle(board[1].x,0,window_size.x, window_size.y,
                             al_map_rgba(border_color.r, border_color.g, 
                                         border_color.b, border_color.a));
}

void drawer::dr_path(double path_radius, vector<p>& path) {
    int n = path.size(); //number of points
    for(int i = 0; i < n; i++) {
        al_draw_filled_circle(path[i].x, path[i].y, path_radius, 
                al_map_rgba(path_color.r, path_color.g, 
                            path_color.b, path_color.a));
    }
    
    for(int i = 1; i < n; i++) {
        al_draw_line(path[i-1].x, path[i-1].y, path[i].x, path[i].y, 
                     al_map_rgba(path_color.r, path_color.g, 
                                 path_color.b, path_color.a),
                     2*path_radius);    
    }
}

void drawer::dr_turrets(vector<turret*>& turrets){
    for(int i = 0; i < turrets.size(); i++) {
        turrets[i] -> dr_turret();
    }
}

void drawer::dr_enemies(vector<enemy*>& enemies) {
    for(int i = 0; i < enemies.size(); i++) {
        enemies[i] -> dr_enemy();
    }
}

void drawer::dr_effects(vector<effect*>& effects) {
   for(int i = effects.size() - 1; i >= 0; i--) {
       effects[i] -> dr_effect(i);
   } 
}

void drawer::dr_stats(double money, int life, int level) {
    dr_money(money);
    dr_life(life);
    dr_level(level);
}

void drawer::dr_life(int life) {
    al_draw_text(fonts[0], al_map_rgba(200,200,200,255), 
            life_position.x, life_position.y, 0,
            ("Life " + to_string(life)).c_str() );
}

void drawer::dr_money(double money) {
    al_draw_text(fonts[0], al_map_rgba(200,200,200,255), 
            money_position.x, money_position.y, 0,
            ("Money " + to_string((int)money)).c_str() );
}

void drawer::dr_level(int level) {
    al_draw_text(fonts[0], al_map_rgba(200,200,200,255), 
            level_position.x, level_position.y, 0,
            ("Level " + to_string(level)).c_str() );
}

void drawer::load_fonts() {
    fonts.push_back(al_load_ttf_font("courier.ttf", 16, 0));
    if(!fonts[0]) {
        fprintf(stderr, "could not load font courier16");
    }
}
