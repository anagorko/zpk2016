#include "turret.h"
#include "cpu.h"
//************************************************************
//turret - base class
bool turret::enemy_in_range(enemy* suspect) {
    double distance = dist( (suspect -> get_position()) , position);
    return (distance  <= get_range());
}

double turret::get_turret_size() {
    return turret_size;
}

p& turret::get_position() {
    return position;
}

void turret::set_position(p _position) {
    position = _position;
}

void turret::dr_range() {
    al_draw_circle(position.x, position.y, get_range(), 
            al_map_rgba(30,30,30,0), 1);
}

turret::~turret() {}

void turret::set_fill(color _fill) {
    fill = _fill;
}

void turret::set_border(color _border) {
    border = _border;
}

//************************************************************
//laser - derived class
laser::laser(cpu* _game, p _position) {
    game = _game;
    position = _position;
    turret_size = game -> get_laser_size();
}

laser::~laser() {}

double laser::get_range(){
    return range;
}

void laser::dr_turret() {
    al_draw_filled_circle(position.x, position.y, turret_size, 
                          al_map_rgba(fill.r,fill.g,fill.b,fill.a));

    al_draw_circle(position.x, position.y, turret_size, 
                   al_map_rgba(border.r,border.g,border.b,border.a),1);

    int size = 5 + turret_level * 2;

    al_draw_triangle(position.x, position.y - size,
                     position.x - size * 0.866, position.y + size / 2,
                     position.x + size * 0.866, position.y + size / 2,
                     al_map_rgba(border.r,border.g,border.b,border.a), 1);
}


void laser::make_shot(vector<enemy*>& enemies, string type) {
    if(delay > 0) {delay--; return;}
    if(enemies.size() == 0){return;}
    if(type == "closest") {
        int closest = -1;
        double closest_distance = 999999;
        double distance;
        for(int i = 0; i < enemies.size(); i++) {
            distance = dist(position, (enemies[i] -> get_position()));
            if (distance < closest_distance) {
                closest = i; 
                closest_distance = distance;
            }
        }
        if(closest_distance <= range) {
            game -> add_laser_effect(position, enemies[closest] -> get_position());
            game -> deal_damage_to_enemy(closest, damage);
            delay = delay_after_shot;
            }
    }
}


//************************************************************
//burn - derived class
burn::burn(p _position) {
    position = _position;
}

burn::~burn() {}

void burn::dr_turret() {
}

void burn::make_shot(vector<enemy*>& enemies, string type) {
}

double burn::get_range(){
    return range;
}
//************************************************************
//lava - derived class
lava::lava(p _position) {
    position = _position;
}

lava::~lava() {}

void lava::dr_turret() {

}

void lava::make_shot(vector<enemy*>& enemies, string type) {
}

double lava::get_range(){
    return range;
}
