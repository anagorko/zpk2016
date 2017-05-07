#include "cpu.h"

cpu::cpu() {
    paint.set_window_size(window_size);
    paint.set_board(board);
}

bool cpu::is_inboard(p& A, vector<p>& board) {
    return board[0].x < A.x &&
           board[0].y < A.y &&
           board[2].x > A.x &&
           board[2].y > A.y;
}

void cpu::display_all() {
    paint.dr_background();
    paint.dr_path(path_radius, path);
    paint.dr_enemies(enemies);
    paint.dr_turrets(turrets);
    paint.dr_effects(effects);
    remove_old_effects();
    paint.dr_border();
    al_flip_display();
}

void cpu::add_laser(p position) {
    turrets.push_back(new laser(this, position));
}

double cpu::get_refresh() {return refresh;}

p cpu::get_start() {return path[0];}

int cpu::get_level() {return level;}

vector<p>& cpu::get_path() {return path;}

void cpu::add_basic_enemy() {
    enemies.push_back(new basic_enemy(this));
}

void cpu::add_laser_effect(p& A, p& B){
    effects.push_back(new laser_effect(A, B));
}

void cpu::enemy_scored(int k){}

void cpu::move_enemies(){
    for(int i = 0; i < enemies.size(); i++) {
        enemies[i] -> move_enemy(path, i);
    }
}

void cpu::deal_damage_to_enemy(int id, double damage) {
    enemies[id] -> deal_damage(id, damage);
}

void cpu::kill_enemy(int id) {
    delete enemies[id];
    enemies.erase(enemies.begin() + id);
}

void cpu::damage_enemies() {
    for(int i = 0; i < turrets.size(); i++) {
        turrets[i] -> make_shot(enemies, shoot_type);
    }
}


void cpu::remove_old_effects() {
    for(int i = effects.size() - 1; i >= 0; i--) {
        if(effects[i] -> get_show_by() < 0) {
            delete effects[i];
            effects.erase(effects.begin() + i);
        }
    }
}
