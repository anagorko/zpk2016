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
        if (path.size() - 1 > segment) {
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

double enemy::get_money() {
    return money;
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
    position = game -> get_start();
    hp = base_hp + base_hp * (game -> get_level()) * (game -> get_level());
    size = base_size + (game -> get_level());
    speed = enemy_speed;
    money = base_money + base_money * (game -> get_level()) / 2;
}


basic_enemy::~basic_enemy(){}
