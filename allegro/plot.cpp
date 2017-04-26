#include<iostream>
using namespace std;

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>

const int w = 800;
const int h = 600;

const double a = -3.14;
const double b = 3.14;

double sup, inf, v[w];

double f(double x)
{
    return sin(x);
}

int zero_x()
{
    return w * (0 - a) / (b - a);
}

int zero_y()
{
    return h * (0 - inf) / (sup - inf);
}


void calculate()
{
    sup = -1000000;
    inf = 1000000;
    
    for (int i = 0; i < w; i++) {
        v[i] = f( a + ((b - a) * i) / w );
        
        if (v[i] > sup) { sup = v[i]; }
        if (v[i] < inf) { inf = v[i]; }
    }
    
    cout << "sup = " << sup << endl;
    cout << "inf = " << inf << endl;
}

int main(int argc, char** argv)
{
    ALLEGRO_DISPLAY       *display = NULL;
 
    al_init();
    al_init_primitives_addon();

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
 
    display = al_create_display(w, h);

    calculate();
    
    al_clear_to_color(al_map_rgb(0,0,0));     
    al_draw_line(0, zero_y(), w, zero_y(), al_map_rgb(255,255,255), 2);
    al_draw_line(zero_x(),0, zero_x(), h, al_map_rgb(255,255,255), 2);

    for (int i = 0; i < w; i++) {
        al_draw_pixel(i, h - (v[i] / (sup - inf) * h + zero_y()), al_map_rgb(255,255,255));
    }
    
    al_flip_display();
    
    al_rest(3);
    al_destroy_display(display);    

    return 0;
}
