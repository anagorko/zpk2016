#include "effect.h"
#include "cpu.h"
//************************************************************
//effect methods

effect::~effect() {}

int effect::get_show_by() {return show_by;}

void effect::decrease_duration() {
    show_by--;
}
//************************************************************
//laser effect methods

laser_effect::laser_effect(p& _A, p& _B){A = _A; B = _B;}
laser_effect::~laser_effect() {}
void laser_effect::dr_effect(int id){ 
    al_draw_line(A.x, A.y, B.x, B.y,
            al_map_rgba(effect_color.r, effect_color.g, 
                        effect_color.b, effect_color.a), line_size);
}

//************************************************************
