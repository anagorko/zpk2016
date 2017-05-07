#include "enemy.h"
#include "cpu.h"

//************************************************************
//enemy - base class

void enemy::move_enemy(vector<p>& path, int k){
    double dist_from_end = dist(position, path[segment]); 
    double move_by = speed/(game -> get_refresh());
    if(dist_from_end > move_by) {
        position = move(position, path[segment], move_by);
    } else 
        if (path.size() > (segment - 1)) {
        position = move(path[segment], path[segment+1], (move_by - dist_from_end));
        segment = segment + 1;}
        else {game -> enemy_scored(k);}
}

p& enemy::get_position() {
    return position;
}

void enemy::deal_damage(int id, double damage) {
    hp = hp - damage;
    if(hp <= 0) {
        game -> kill_enemy(id);
    }
}

enemy::~enemy(){}

//************************************************************
//basic enemy - derived class

void basic_enemy::dr_enemy() {
    al_draw_filled_circle(position.x, position.y, size,
            al_map_rgba(enemy_color.r, enemy_color.g, 
                enemy_color.b, enemy_color.a));
    
    al_draw_circle(position.x, position.y, size,
            al_map_rgba(enemy_color.r, enemy_color.g, 
                enemy_color.b, enemy_color.a), 2);
        
}

basic_enemy::basic_enemy(cpu* _game) {
    game = _game;
    segment = 1;
    position = game -> get_start();
    speed = 100;
    hp = 10 + 10 * game -> get_level();
    size = 10 + game -> get_level();
    enemy_color = color(120,120,0,128);
}


basic_enemy::~basic_enemy(){}
