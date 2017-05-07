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
        int segment;
        double speed;
        double hp;
        double size;
        color enemy_color;
    public:
        virtual void dr_enemy() = 0;
        void move_enemy(vector<p>& path, int k);
        void deal_damage(int id, double damage);
        virtual ~enemy();
        
        p& get_position();

};

class basic_enemy : public enemy {
    private:

    public:
        basic_enemy(cpu* _game);
        virtual void dr_enemy();
        ~basic_enemy();
};

#endif

