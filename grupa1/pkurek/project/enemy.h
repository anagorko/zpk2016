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
    public:
        virtual void dr_enemy() = 0;
        void move_enemy(vector<p>& path, int k);
        void deal_damage(int id, double damage);
        virtual ~enemy();
        
        p& get_position();

};

class basic_enemy : public enemy {
    private:
        double base_hp = 10;
        double base_size = 10; 
        double enemy_speed = 50;
        color enemy_color = color(120, 120, 0, 128);

    public:
        basic_enemy(cpu* _game);
        virtual void dr_enemy();
        ~basic_enemy();
};

#endif

