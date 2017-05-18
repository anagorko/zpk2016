#ifndef ENEMY_H
#define ENEMY_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "others.h"
#include <math.h>
#include <vector>
using namespace std;

//forward declarations
class cpu;

//abc enemy
class enemy {
    protected:
        cpu* game;
        p position;
        int segment = 1;
        double hp;
        double size;
        double speed;
        double money;
        double start_hp;
        double start_size;
        
    public:
        virtual void dr_enemy() = 0;
        void move_enemy(vector<p>& path, int k);
        void deal_damage(int id, double damage);
        double get_money();
        virtual ~enemy();
        
        p& get_position();

};

//************************************************************
//basic_enemy - derived class
class basic_enemy : public enemy {
    private:
        double base_hp = 2;
        double base_size = 10;
        double enemy_speed = 50;
        double base_money = 1;

        color enemy_color = color(120, 120, 0, 128);

    public:
        basic_enemy(cpu* _game);
        virtual void dr_enemy();
        virtual ~basic_enemy();
};

//************************************************************
//speeder - derived class
class speeder : public enemy {
    private:
        double base_hp = 0.25;
        double base_size = 3;
        double enemy_speed = 200;
        double base_money = 0.25;

        color enemy_color = color(0,153,153,255);

    public:
    
        speeder(cpu* _game);
        virtual void dr_enemy();
        virtual ~speeder();
};

#endif

