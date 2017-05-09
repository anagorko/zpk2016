#ifndef CPU_H
#define CPU_H

#include "others.h"
#include "turret.h"
#include "enemy.h"
#include "effect.h"
#include <vector>
#include <string>
#include "drawer.h"
using namespace std;

//forward declarations:
class turret;
class enemy;
class effect;
class drawer;

//class cpu:
class cpu {

    private:
        int lives = 20;
        int level = 1;
        double speed = 10;
        double money = 100;
        int show_range = 1;
        p window_size = p(800,600);
        double path_radius = 30;
        double refresh = 60; //times per sec
        string shoot_type = "closest";

        double laser_cost = 30;

        vector<p> board = {p(100,100), p(700, 100), p(700,500), p(100, 500)}; 
            // vector of four points - clockwise
        vector<p> path = {p(100,100), p(700,300), p(200,300), p(600,500)};  
            // vector of n points - enemy path

        drawer paint;
        vector<turret*> turrets;
        vector<enemy*> enemies;
        vector<effect*> effects;

    public:
       cpu();

       bool is_inboard(p& A, vector<p>& board);
       void display_all();

       void add_laser(p position);
       void add_basic_enemy();
       void add_laser_effect(p& A, p& B);

       void move_enemies();
       void damage_enemies();
       
       void enemy_scored(int k);
       void deal_damage_to_enemy(int closest, double damage);
       void kill_enemy(int id);
       void remove_old_effects();

       void add_level();


       //returns game statistics
       double get_refresh();
       p get_start();
       int get_level();
       int get_lives();
       vector<p>& get_path();

};

#endif
