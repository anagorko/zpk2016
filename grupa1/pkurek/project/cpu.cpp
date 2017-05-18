#include "cpu.h"
#include "drawer.h"

cpu::cpu() {
    paint = drawer(this);
    paint.set_window_size(window_size);
    paint.set_board(board);
    paint.load_fonts();
}

bool cpu::is_turret_offboard(double turret_size, p turret_center) {
    return !(((board[0].x + turret_size) < turret_center.x) &&
           ((board[0].y + turret_size) < turret_center.y) &&
           ((board[2].x - turret_size) > turret_center.x) &&
           ((board[2].y - turret_size) > turret_center.y));
}

bool cpu::is_turret_onpath(double turret_size, p turret_center) {
    double dist_center_from_path = 99999;
    double dist_center_from_segment;
    for(int i = 1; i < path.size(); i++) {
        dist_center_from_segment = 
            dist_from_segment(path[i-1] , path[i], turret_center);
        if(dist_center_from_segment < dist_center_from_path) {
            dist_center_from_path = dist_center_from_segment;
        }
    }
    return (dist_center_from_path < (turret_size + path_radius));
}

bool cpu::is_turret_onturret(double turret_size, p turret_center) {
    for(int i = 0; i < turrets.size(); i++) {
        if((turrets[i]->get_turret_size() + turret_size) > 
                dist((turrets[i]->get_position()), turret_center)) {
            return true;
        }
    }
    return false;
}

bool cpu::is_turret_coliding(double turret_size, p turret_center) {
    if(is_turret_onpath(turret_size, turret_center)) {return true;}
    if(is_turret_offboard(turret_size, turret_center)) {return true;}
    if(is_turret_onturret(turret_size, turret_center)) {return true;}
    return false;
}

void cpu::display_all() {
    paint.dr_background();
    paint.dr_path(path_radius, path);
    paint.dr_enemies(enemies);
    paint.dr_turrets(turrets);
    paint.dr_ranges(turrets);
    paint.dr_effects(effects);
    draw_placer();
    paint.dr_border();
    paint.dr_stats(money, lives, level);
    al_flip_display();
}

void cpu::display_help() {
    paint.dr_background();
    paint.dr_path(path_radius, path);
    paint.dr_enemies(enemies);
    paint.dr_turrets(turrets);
    paint.dr_ranges(turrets);
    paint.dr_effects(effects);
    paint.dr_help();
    draw_placer();
    paint.dr_border();
    paint.dr_stats(money, lives, level);
    al_flip_display();
}

void cpu::add_laser(p position) {
    if(is_turret_coliding(laser_size, position)) {return;}
    if(money - laser_cost > 0) {
       money = money - laser_cost;
       turrets.push_back(new laser(this, position));
    }
}

void cpu::add_basic_enemy() {
    enemies.push_back(new basic_enemy(this));
}

void cpu::add_speeder() {
    enemies.push_back(new speeder(this));
}

double cpu::get_refresh() {return refresh;}

p cpu::get_start() {return path[0];}

int cpu::get_level() {return level;}

vector<p>& cpu::get_path() {return path;}

int cpu::get_show_range() {return show_range;}


void cpu::add_laser_effect(p& A, p& B){
    effects.push_back(new laser_effect(A, B));
}

void cpu::enemy_scored(int k){
    lives--;
    kill_enemy(k);
}

void cpu::make_turn() {
    move_enemies();
    damage_enemies();
    decrease_effect_durability();
    unleash();
}

void cpu::move_enemies(){
    for(int i = 0; i < enemies.size(); i++) {
        enemies[i] -> move_enemy(path, i);
    }
}

void cpu::deal_damage_to_enemy(int id, double damage) {
    enemies[id] -> deal_damage(id, damage);
}

void cpu::kill_enemy(int id) {
    money = money + (enemies[id] -> get_money());
    delete enemies[id];
    enemies.erase(enemies.begin() + id);
}

void cpu::damage_enemies() {
    for(int i = 0; i < turrets.size(); i++) {
        turrets[i] -> make_shot(enemies, shoot_type);
    }
}

void cpu::decrease_effect_durability() {
    //decreases effect durability:
    for(int i = 0; i < effects.size(); i++) {
        effects[i] -> decrease_duration();
    }
    //removes old effects:
    for(int i = effects.size() - 1; i >= 0; i--) {
        if(effects[i] -> get_show_by() < 0) {
            delete effects[i];
            effects.erase(effects.begin() + i);
        }
    }
}

void cpu::unleash() {
    if(level % 2 == 1) {
        unleash_weave_of_basic_enemies();
    } else {
    unleash_weave_of_speeders();
    }
}

void cpu::unleash_weave_of_basic_enemies() {
    if((turn % 80 == 1) && unleash_enemies) {
        add_basic_enemy();
    }
    if(turn % 800 == 0) {
        unleash_enemies = false;}
    if(weave_in_progress && (enemies.size() == 0) && (!unleash_enemies)) {
        level++;    
        weave_in_progress = false;
    }
    turn++;
}

void cpu::unleash_weave_of_speeders() {
    if((turn % 20 == 1) && unleash_enemies) {
        add_speeder();
    }
    if(turn % 800 == 0) {
        unleash_enemies = false;}
    if(weave_in_progress && (enemies.size() == 0) && (!unleash_enemies)) {
        level++;    
        weave_in_progress = false;
    }
    turn++;
}

void cpu::unleash_enemies_true() {
    if(!weave_in_progress) {
        turn = 1;
        unleash_enemies = true;
        weave_in_progress = true;
    }
}

int cpu::get_lives() {
    return lives;
}

double cpu::get_laser_size() {
    return laser_size;
}

void cpu::change_show_range_state() {
    if(show_range == 1) {show_range = 0;} else
    if(show_range == 0) {show_range = 1;}
}

void cpu::draw_placer() {
    if(placer.size() > 0) {
        placer[0] -> dr_turret();
        placer[0] -> dr_range();
    }
}

void cpu::add_placer(string type) {
    remove_placer();
    if(type == "laser") {
        placer.push_back(new laser(this, p(0,0)));
    }
}

void cpu::remove_placer() {
    if(placer.size() > 0) {
        delete placer[0];
        placer.clear();
    }
}

void cpu::set_placer(p position) {
    if(placer.size() > 0) {

        placer[0] -> set_position(position);

        if(is_turret_coliding((placer[0] -> get_turret_size()) , position)) {
            placer[0] -> set_fill(cant_place_fill);
        } else {
            placer[0] -> set_fill(can_place_fill);
        }

    }
}
