#ifndef TURRET_H
#define TURRET_H
#include "others.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <vector>
#include "enemy.h"
using namespace std;

//forward declarations:
class cpu;

//abc turret:
class turret {
    protected: 
        cpu* game;
        p position;
        int turret_level = 1;
        double damage = 20;
        double turret_size;
        int delay = 0;
        int delay_after_shot = 120;

    public:
        virtual double get_range() = 0;
        virtual void dr_turret() = 0;
        virtual void make_shot(vector<enemy*>& enemies, string type) = 0;
        bool enemy_in_range(enemy* suspect); 
        double get_turret_size();
        p& get_position();
        void dr_range();

};

//derived classes:
class laser : public turret {
    private:
        double range = 100;

    public:
        laser(cpu* _game, p _position);
        virtual void dr_turret();
        virtual void make_shot(vector<enemy*>& enemies, string type);
        virtual double get_range();

};

class burn : public turret {
    private:
        double range = 100;

    public:
        burn(p _position);
        virtual void dr_turret();
        virtual void make_shot(vector<enemy*>& enemies, string type);
        virtual double get_range();
};

class lava : public turret {
    private:
        double range = 100;

    public:
        lava(p _position);
        virtual void dr_turret();
        virtual void make_shot(vector<enemy*>& enemies, string type);
        virtual double get_range();

};

#endif
