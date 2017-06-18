#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <math.h>
#include <random>

#include <iostream>
using namespace std;

void * _Unwind_Resume =0;
void * __gxx_personality_v0=0;

// Konfiguracja:

const int sz = 1250;
const int wy = 750;

const int sz_b = 5;
const int wy_b = 10;

const float fps = 60;

const float g = 0.002;

const int n_land = 3;
const int sky = 0.3*wy;
const int max_land_size = sz_b*10; //maksymalna szerokosc ladowiska
const float ship_size = 1.5;

bool crash = false;
bool landed = false;

const int init_fuel = 1000;
const float a = 0.008;

bool key[ALLEGRO_KEY_MAX];

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;


class ship{
    float x[2]; //pozycja
    float r; //obrot
    float v[2]; //predkosc
    int fuel; //paliwo

    float b[2]; //pozycja dziobu
    float lw[2]; //pozycja lewego skrzydla
    float rw[2]; //pozycja prawego skrzydla

    bool burn;
    int energy;

public:

    ship(){
        x[0] = (int)(sz*0.5);
        x[1] = 30;
        r = 0;
        v[0]=0;
        v[1]=0;
        fuel = init_fuel;
        burn = false;
        energy = 1000;
        shape();
    }

    void shape(){
        b[0] = x[0] - sin(r)*7*ship_size;
        b[1] = x[1] - cos(r)*7*ship_size;

        lw[0] = x[0] - sin(r+2)*5*ship_size;
        lw[1] = x[1] - cos(r+2)*5*ship_size;

        rw[0] = x[0] - sin(r-2)*5*ship_size;
        rw[1] = x[1] - cos(r-2)*5*ship_size;
    }

    void drift(){
        x[0] = x[0] + v[0];
        x[1] = x[1] + v[1];
        v[1] = v[1] + g;
        shape();
    }

    void acc(){
        v[0] = v[0] - sin(r)*a;
        v[1] = v[1] - cos(r)*a;
        fuel = fuel - 1;
        burn = true;
    }

    void rot(float _r){
        r = r + 0.1*_r;
        if(r>2*M_PI || r<-2*M_PI){
            r=0;
        }
        if(r<0.0999 && r>0){
            r = 0;
        }
        if(r>-0.0999 && r<0){
            r = 0;
        }
        energy = 0;
    }

    void draw(){
        al_draw_pixel(x[0], x[1], al_map_rgb(255,255,255));
        al_draw_line(x[0], x[1], lw[0], lw[1], al_map_rgb(255,255,255), 1);
        al_draw_line(x[0], x[1], rw[0], rw[1], al_map_rgb(255,255,255), 1);
        al_draw_line(b[0], b[1], lw[0], lw[1], al_map_rgb(255,255,255), 1);
        al_draw_line(b[0], b[1], rw[0], rw[1], al_map_rgb(255,255,255), 1);

        if(burn){
            float r=(rand()%50)*0.01+0.5;
            al_draw_line((lw[0]+x[0])*0.5, (lw[1]+x[1])*0.5, x[0]*(1+r)-b[0]*r, x[1]*(1+r)-b[1]*r, al_map_rgb(255,255,255), 1);
            al_draw_line((x[0]+rw[0])*0.5, (x[1]+rw[1])*0.5, x[0]*(1+r)-b[0]*r, x[1]*(1+r)-b[1]*r, al_map_rgb(255,255,255), 1);
            burn = false;
        }
    }

    bool checkIfOutOfBonds(){
        if(x[0]<0 || x[0]>sz || x[1]>wy){
            return true;
        }
        return false;
    }

    bool checkIfCrashed(float _h[sz+1]){
        if((lw[1]>_h[(int)lw[0]] || rw[1]>_h[(int)rw[0]] || b[1]>_h[(int)b[0]])){
            v[0]=0;
            v[1]=0;
            return true;
        }
    return false;
    }

    bool checkIfLanded(float _h[sz+1], bool _l[sz+1]){
        if((int)lw[1]==(int)rw[1] && (int)lw[1]>_h[(int)lw[0]]-1 && _l[(int)lw[0]] && _l[(int)rw[0]] && v[1]<g*300){
            return true;
        }
        return false;
    }

    void go(){
        energy++;
        if(key[ALLEGRO_KEY_LEFT] && energy>1){
            rot(1);
        }
        if(key[ALLEGRO_KEY_RIGHT] && energy>1){
            rot(-1);
        }
        if(key[ALLEGRO_KEY_UP] && fuel>0){
            acc();
        }
    }

    int getFuel(){
        return fuel;
    }


};

struct terrain{
    float h[sz+1];
    bool l[sz+1];

    terrain(int n, int m){
        for(int i=0;i<=sz+1;i++){
            l[i]=false;
        }
        for(int i=0;i<n;i++){
            int a = (int)(i*sz/n)+rand()%((int)(sz/n)-m);
            int b = a+rand()%((int)(m*0.5))+(int)(m*0.5);
            for(int j=a;j<=b;j++){
                l[j]=true;
            }
        }
        h[0]=sky+rand()%(wy-sky);
        float t[5];
        for(int i=0;i<5;i++){
            t[i]=0.5*(rand()%wy_b-rand()%wy_b);
        }
        for(int i=1;i<sz+1;i++){
            if(i%sz_b==0){
                t[0]=t[1];
                t[1]=t[2];
                t[2]=t[3];
                t[3]=t[4];
                t[4]=0.18*(t[0]+t[1]+t[2]+t[3])+0.5*(rand()%wy_b-rand()%wy_b);
            }
            if(l[i]){
                h[i]=h[i-1];
            }
            else{
                h[i]=h[i-1]+t[0];
            }
            if(h[i]>wy){
                h[i]=wy;
                for(int i=0;i<4;i++){
                    t[i]=-rand()%wy_b;
                }
                t[4]=-t[4];
            }
            if(h[i]<sky){
                h[i]=sky;
                for(int i=0;i<4;i++){
                    t[i]=rand()%wy_b;
                }
                t[4]=-t[4];
            }
        }
    };
};

class board{
    terrain ter;
    ship s;

public:

    board(int n, int m):ter(n,m){};

    void update(){
        s.go();

        //srawdzenie czy statek nie wylecial za plansze
        if(s.checkIfOutOfBonds()){
            crash = true;
        }

        //sprawdzenie czy statek wyladowal
        if(s.checkIfLanded(ter.h, ter.l)){
            landed = true;
        }
        //sprawdzenie czy statek sie rozbil
        else if(s.checkIfCrashed(ter.h)){//} && !p.ter.l[x_x]){
            crash = true;
        }
        //aktualizacja polozenia i predkosci statku
        s.drift();
    }

    void draw_board(){
        al_clear_to_color(al_map_rgb(0,0,0));

        float f = 100*(init_fuel-s.getFuel())/init_fuel;
        al_draw_rectangle(sz-sz_b, wy_b, sz-sz_b-10, wy_b+100, al_map_rgb(255,255,255), 0);
        al_draw_filled_rectangle(sz-sz_b, wy_b+f, sz-sz_b-10, wy_b+100, al_map_rgb(255,255,255));

        // rysuje teren:
        for (int i = 0; i<sz; i++){
            al_draw_line(i, ter.h[i], (i+1), ter.h[i+1], al_map_rgb(255,255,255), 1+2*(ter.l[i]+ter.l[i+1]-1));
        }

        // rysuje statek:
        s.draw();
    }
};

int init(){
    srand(time(NULL));

    if(!al_init()) {
        cerr << "Blad podczas inicjalizacji allegro." << endl;
        return -1;
    }
    if (!al_init_primitives_addon()) {
        cerr << "Blad podczas inicjalizacji dodatku 'primitives'." << endl;
        return -1;
    }
    if(!al_install_keyboard()) {
        cerr << "Blad podczas inicjalizacji klawiatury." << endl;
        return -1;
    }
    timer = al_create_timer(1.0 / fps);
    if(!timer) {
        cerr << "Blad podczas inicjalizacji zegara." << endl;
        return -1;
    }
    display = al_create_display(sz, wy);
    if(!display) {
        cerr << "Blad podczas inicjalizacji ekranu." << endl;
        al_destroy_timer(timer);
        return -1;
    }
    event_queue = al_create_event_queue();
    if(!event_queue) {
        cerr << "Blad podczas inicjalizacji kolejki zdarzen." << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_clear_to_color(al_map_rgb(0,0,0));

    al_flip_display();
    al_start_timer(timer);

    al_init_font_addon();
    al_init_ttf_addon();
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    return 0;
}

int main()
{
    if (init() != 0) {
        cerr << "Inicjalizacja nie powiodla sie." << endl;
        return -1;
    }

    ALLEGRO_FONT *font1 = al_load_ttf_font("courier.ttf", -48, 0);
    ALLEGRO_FONT *font2 = al_load_ttf_font("courier.ttf", -36, 0);
    ALLEGRO_FONT *font3 = al_load_ttf_font("courier.ttf", -24, 0);
    ALLEGRO_FONT *font4 = al_load_ttf_font("courier.ttf", -12, 0);

    al_draw_text(font1, al_map_rgb(255,255,255), sz/2, 100, ALLEGRO_ALIGN_CENTRE, "Lunar lander");
    al_draw_text(font4, al_map_rgb(255,255,255), sz/2, 160, ALLEGRO_ALIGN_CENTRE, "by Jan Woznica");
    al_draw_text(font2, al_map_rgb(255,255,255), sz/2, wy*0.4+60, ALLEGRO_ALIGN_CENTRE, "New game? y/n");

    al_flip_display();

    bool start=false;
    bool exit=false;
    int win_count=0;

    while(!exit){
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            key[ev.keyboard.keycode] = true;
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            key[ev.keyboard.keycode] = false;
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE || ev.keyboard.keycode == ALLEGRO_KEY_N) {
                exit = true;
            }else if(ev.keyboard.keycode == ALLEGRO_KEY_Y){
                start = true;
            }
        }

        if(start){
            crash = false;
            landed = false;
            board p(n_land, max_land_size);
            p.draw_board();
            bool redraw = true;
            bool wait = false;

            while(!crash && !landed){
                ALLEGRO_EVENT ev;
                al_wait_for_event(event_queue, &ev);
                if(ev.type == ALLEGRO_EVENT_TIMER && !wait) {
                    redraw = true;
                    p.update();
                } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                    key[ev.keyboard.keycode] = true;
                } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                    key[ev.keyboard.keycode] = false;
                    if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        crash = true;
                    }else if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE){
                        if(wait==true){
                            wait = false;
                        }else{
                            wait = true;
                        }
                    }
                }
                if(redraw && al_is_event_queue_empty(event_queue)) {
                    redraw = false;
                    p.draw_board();
                    al_flip_display();
                }
            }
            if(landed){
                win_count++;
                al_draw_text(font1, al_map_rgb(255,255,255), sz/2, wy*0.4, ALLEGRO_ALIGN_CENTRE, "You've landed successfully.");
                al_draw_textf(font2, al_map_rgb(255,255,255), sz/2, wy*0.4+60, ALLEGRO_ALIGN_CENTRE, "%d consecutive wins!", win_count);
                al_draw_text(font2, al_map_rgb(255,255,255), sz/2, wy*0.4+120, ALLEGRO_ALIGN_CENTRE, "Once more? y/n");
                al_flip_display();
            }
            if(crash){
                al_draw_text(font1, al_map_rgb(255,255,255), sz/2, wy*0.4, ALLEGRO_ALIGN_CENTRE, "You failed!");
                al_draw_textf(font2, al_map_rgb(255,255,255), sz/2, wy*0.4+60, ALLEGRO_ALIGN_CENTRE, "After %d consecutive wins.", win_count);
                al_draw_text(font2, al_map_rgb(255,255,255), sz/2, wy*0.4+120, ALLEGRO_ALIGN_CENTRE, "New game? y/n");
                al_flip_display();
                win_count=0;
            }
            start=false;
        }
    }

    al_destroy_display(display);

    return 0;
}
