#ifndef EFFECT_H
#define EFFECT_H
#include "others.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

//forward declarations:
class cpu;

//abc - effect:
class effect {
    protected:
        cpu* game;
        int show_by = 10;

    public:
        virtual void dr_effect(int id) = 0;
        virtual ~effect();
        int get_show_by();
};

//derived classes:
class laser_effect : public effect {
    protected:
        color effect_color = color(255,0,0,128);
        p A, B;
        double line_size = 1;
    public:
        laser_effect(p& _A, p& _B);
        virtual void dr_effect(int id);
        virtual ~laser_effect();
};






#endif

