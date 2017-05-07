#ifndef DRAWER_H
#define DRAWER_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "others.h"
#include "turret.h"
#include "enemy.h"
#include "effect.h"
#include <vector>
using namespace std;

//forward declarations:
class cpu;

//class
class drawer {
    private:
        color bg_color = color(130,0,0,255);
        color border_color = color(0,0,150,255);
        color path_color = color(50,0,0,255);
        p window_size;
        vector<p> board;

    public:
        drawer();

        void dr_background();
        void dr_border();
        void dr_path(double path_radius, vector<p>& path);
        void dr_turrets(vector<turret*>& turrets);
        void dr_enemies(vector<enemy*>& enemies);
        void dr_effects(vector<effect*>& effects);

        void set_window_size(p _window_size);
        void set_board(vector<p>& board);

};

#endif
