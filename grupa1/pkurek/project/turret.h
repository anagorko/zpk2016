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
        double base_turret_size = 15;
        int delay = 0;
        int delay_after_shot = 120;
        double range = 0;

    public:
        virtual void dr_turret() = 0;
        virtual void make_shot(vector<enemy*>& enemies, string type) = 0;
        bool enemy_in_range(enemy* suspect); 

};

//derived classes:
class laser : public turret {
    private:
        double range = 100;

    public:
        laser(cpu* _game, p _position);
        virtual void dr_turret();
        virtual void make_shot(vector<enemy*>& enemies, string type);

};

class burn : public turret {
    private:

    public:
        burn(p _position);
        virtual void dr_turret();
        virtual void make_shot(vector<enemy*>& enemies, string type);
};

class lava : public turret {
    private:

    public:
        lava(p _position);
        virtual void dr_turret();
        virtual void make_shot(vector<enemy*>& enemies, string type);

};

#endif
