#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include <iostream>
#include <cstdlib>


using namespace std;


bool key[ALLEGRO_KEY_MAX];

class kwadrat{
    int kx,ky,kr,kolor;

public:
    kwadrat(){
    kx=0;
    ky=0;
    kr=1;
    kolor=1;}

    kwadrat(int x,int y, int r){
    kx=x;
    ky=y;
    kr=r;
    }

    int getX(){return kx;}
    int getY(){return ky;}
    int getR(){return kr;}
    int getK(){return kolor;}
    void setX(int x){kx=x;}
    void setY(int y){ky=y;}
    void setR(int r){kr=r;}
    void setK(int kol){kolor=kol;}
};

vector <kwadrat> pamiec;    //wektor pol na ktorych mrowka juz byla


//ladowanie allegro
//**to jest fragment kodu skopiowany z stackoverflow - ladowanie obrazka w okreslonym rozmiarze

ALLEGRO_BITMAP *load_bitmap_at_size(const char *filename, int w, int h)
{
  ALLEGRO_BITMAP *resized_bmp, *loaded_bmp, *prev_target;


  resized_bmp = al_create_bitmap(w, h);
  if (!resized_bmp) return NULL;


  loaded_bmp = al_load_bitmap(filename);
  if (!loaded_bmp)
  {
     al_destroy_bitmap(resized_bmp);
     return NULL;
  }

  prev_target = al_get_target_bitmap();
  al_set_target_bitmap(resized_bmp);

  al_draw_scaled_bitmap(loaded_bmp,
     0, 0,
     al_get_bitmap_width(loaded_bmp),
     al_get_bitmap_height(loaded_bmp),
     0, 0,
     w, h,
     0
  );


  al_set_target_bitmap(prev_target);
  al_destroy_bitmap(loaded_bmp);

  return resized_bmp;
}

//definicja stron swiata
enum strona{
N=0,
E=1,
S=2,
W=3};



class mrowka{

    int mx, my, rozmiar;
    strona zwrot;
    ALLEGRO_BITMAP *fota;

public:
    mrowka(){
           mx=0;
           my=0;
           rozmiar=1;
           zwrot=N;
    }

// to jest nowa mrowka
mrowka(int x, int y, int r){
    mx=x;
    my=y;
    rozmiar=r;
    zwrot=N;
    fota = load_bitmap_at_size("/Users/romek/Desktop/mrowka.jpg",80,80);

}
//odczytywanie i zmiana parametrow mrowki
int getX(){return mx;}
int getY(){return my;}
int getR(){return rozmiar;}
strona getZ(){return zwrot;}
void setX(int x){mx=x;}
void setY(int y){my=y;}
void setR(int r){rozmiar=r;}
void setZ(strona s){zwrot=s;}
ALLEGRO_BITMAP *getBMP(){return fota;}


//mrowka obraca sie w prawo
void prawo(){
    switch(zwrot){
case N:
    zwrot=E; break;
case E:
    zwrot=S; break;
case S:
    zwrot=W; break;
case W:
    zwrot=N; break;
    }
}

void lewo(){
    switch(zwrot){
case S:
    zwrot=E; break;
case W:
    zwrot=S; break;
case N:
    zwrot=W; break;
case E:
    zwrot=N; break;
    }
}

void krok(){
     switch(zwrot){
case N:
    my-=rozmiar; break;
case E:
    mx+=rozmiar; break;
case S:
    my+=rozmiar; break;
case W:
    mx-=rozmiar; break;
    }
}

void obrot(vector <kwadrat> &pamiec){
//1- czarny **mozna dodac wiecej**
//bialy - w prawo
//czarny - w lewo


for(int i=0;i<pamiec.size();++i){
    if(mx==pamiec[i].getX() && my==pamiec[i].getY()) {lewo(); return;}
}

prawo();

}

void zmienkolor(vector <kwadrat> &pamiec){
// zmien kolor kwadratu **dodaj lub usun z pamieci**
        //jesli znadje, usuwam z pamieci
        for(int i=0; i<pamiec.size();++i){

    if(pamiec[i].getX()==mx && pamiec[i].getY()==my) {
            pamiec.erase(pamiec.begin()+i);
    return;
    }
    }

    //jesli nie znajde dodaje do pamieci
    kwadrat nowy(mx,my,rozmiar);
    pamiec.push_back(nowy);
    return;

}

int czywyjdzie(){
    int w=800;
    int h=600;

    //mrowka moze wyjsc z czterech stron planszy, sprawdzam czy nie jest ustawiona na granicy
    //jesli pozycja + rozmiar (nastepny krok) spowoduje wyjscie to zwracam odpowiednia cyfre
    switch(zwrot){
case N:
    if(my-rozmiar<0 ) return 1;
    break;
case E:
    if(mx+rozmiar>w) return 2;
    break;
case S:
    if(my+rozmiar>h) return 3;
    break;
case W:
    if(mx-rozmiar<0) return 4;
    }

    return 0;

}

};

bool czywyjda(vector <mrowka> mrowisko){
for(int i=0; i<mrowisko.size();++i){
    if (mrowisko[i].czywyjdzie()>0) return false;

}
return true;
}

// deiniuje dodatkowo wektor mrowek ktore bedzie mozna stawiac

vector <mrowka> mrowisko;

//dodanie nowej mrowki
void dodajMrowke(vector <mrowka> &mrowisko, int x, int y){
int r=80;
x=floor(x/r)*r;
y=floor(y/r)*r;

for(int i=0; i<mrowisko.size();++i){
    //jesli jest tu mrowka to ja usun
    if(mrowisko[i].getX()==x && mrowisko[i].getY()==y)
    {mrowisko.erase(mrowisko.begin()+i);
    return;
    }
    }
//jesli nie ma to dodaj
mrowka nowa(x,y,r);
mrowisko.push_back(nowa);
}

void obracajMrowki(vector <mrowka> mrowisko, vector <kwadrat> &pamiec){
for(int i=0;i<mrowisko.size();++i) mrowisko[i].obrot(pamiec);
}

void zmieniajKwadraty(vector <mrowka> mrowisko, vector <kwadrat> pamiec){
for(int i = 0; i < mrowisko.size(); ++i)
            mrowisko[i].zmienkolor(pamiec);
}

//dodanie nowego kwadratu
void dodajKwadrat(vector <kwadrat> &pamiec, int x, int y){

int r=80;
x=floor(x/r)*r;
y=floor(y/r)*r;

for(int i=0; i<pamiec.size();++i){

    if(pamiec[i].getX()==x && pamiec[i].getY()==y) {
            pamiec.erase(pamiec.begin()+i);
    return;
    }
    }
//jesli nie ma to dodaj
kwadrat nowy(x,y,r);
pamiec.push_back(nowy);
}

/////to by³a czêœæ techniczna. teraz t³umaczenie tego na grafikê

//1. narysuj kwadraty z pamieci

void rysujKwadraty(vector <kwadrat> pamiec){

for(int i=0;i<pamiec.size();++i){
    int r=pamiec[i].getR();
    int x=pamiec[i].getX();
    int y=pamiec[i].getY();
    al_draw_filled_rectangle(x, y, x + r, y + r, al_map_rgb(0,0,0));
}

}

//2. narysuj mrowke

void rysujMrowki(vector <mrowka> mrowisko){
    for(int i=0;i<mrowisko.size();++i){
        int x = mrowisko[i].getX();
        int y=mrowisko[i].getY();
        int r=mrowisko[i].getR();
        float obrot=mrowisko[i].getZ() * M_PI / 2;
        al_draw_rotated_bitmap(load_bitmap_at_size("/Users/romek/Desktop/mrowka.jpg",r,r),r/2,r/2,x+r/2,y+r/2,obrot,0);
    }
}

//znajdz mrowke najblizej srodka i zwroc jej numer w tablicy
int srodkowa(vector <mrowka> mrowisko){
    int x=1000;
    int s=0;
//if (mrowisko.size()==1) return s;
for (int i=0; i<mrowisko.size();++i){
    if(sqrt((400-mrowisko[i].getX())^2 + (300-mrowisko[i].getY())^2)<x) {
        x=sqrt((400-mrowisko[i].getX())^2 + (300-mrowisko[i].getY())^2);
        s=i;
    }
}

return s;

}

//SKALOWANIE
void zmniejsz(vector <mrowka> &mrowisko, vector <kwadrat> &pamiec){
// O ILE
    int w=800;
    int h=600;
    int s=srodkowa(mrowisko);
    int sr=mrowisko[s].getR();
    int nr=sr-4;
	int srodx=mrowisko[s].getX();
	int srody=mrowisko[s].getY();


	for(int i=0; i<mrowisko.size();++i){
        mrowisko[i].setR(nr);
        mrowisko[i].setX(((mrowisko[i].getX()-srodx)/sr)*nr+400);
        mrowisko[i].setY(((mrowisko[i].getY()-srody)/sr)*nr+300);
	}

	for(int i=0; i<pamiec.size();++i){
        pamiec[i].setR(nr);
        pamiec[i].setX(((pamiec[i].getX()-srodx)/sr)*nr+400);
        pamiec[i].setY(((pamiec[i].getY()-srody)/sr)*nr+300);

	}
}

void powieksz(vector <mrowka> &mrowisko, vector <kwadrat> &pamiec){
    int w=800;
    int h=600;
    int s=srodkowa(mrowisko);
    int sr=mrowisko[s].getR();
    int nr=sr+4;
	int srodx=mrowisko[s].getX();
	int srody=mrowisko[s].getY();

	for(int i=0; i<mrowisko.size();++i){
        mrowisko[i].setR(nr);
        mrowisko[i].setX(((mrowisko[i].getX()-srodx)/sr)*nr+srodx);
        mrowisko[i].setY(((mrowisko[i].getY()-srody)/sr)*nr+srody);
	}

	for(int i=0; i<pamiec.size();++i){
        pamiec[i].setR(nr);
        pamiec[i].setX(((pamiec[i].getX()-srodx)/sr)*nr+srodx);
        pamiec[i].setY(((pamiec[i].getY()-srody)/sr)*nr+srody);

	}

}


int main(int argc, char **argv){

    int rozmiar = 80;
    int kolory = 4;
    int szer = 800;
    int wys = 600;
    int FPS = 1;
    int iter = 0;
    int tmpRoz = 80;
    bool zwiekszPredkosc = false;


    //zmienne do petli
    bool done = false;
    bool redraw = true;
    bool done1 = false;
    bool pow=false;
    bool zm=false;



    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;


    al_init();
    if(!al_init())
        return -1;

    display = al_create_display(szer, wys); //utworzenie wyswietlacza

    if(!display)
        return -1;

           //instalacja addon—w
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_mouse();

    event_queue = al_create_event_queue();


    al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());


    ALLEGRO_EVENT ev;

    al_set_window_title(display , "MROWKA");

    mrowisko.clear();
    pamiec.clear();
    al_clear_to_color(al_map_rgb(255,255,255));
    al_flip_display();
    timer = al_create_timer(1.0 / FPS);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    START:
    al_start_timer(timer);


    //petla do wstepnej edycuji - dodawania mrowek i kwadratow
    while(!done)
    {
        al_clear_to_color(al_map_rgb(255,255,255));

        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
            done1 = true;

        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {

            ALLEGRO_MOUSE_STATE klawisz;
            al_get_mouse_state(&klawisz);

            if(klawisz.buttons & 1) dodajMrowke(mrowisko, ev.mouse.x, ev.mouse.y);
            else if(klawisz.buttons & 2) dodajKwadrat(pamiec, ev.mouse.x, ev.mouse.y);


        }
        else if(ev.type == ALLEGRO_EVENT_TIMER) {redraw = true;}

		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                done = true;
                done1 =true;
            }

            if(ev.keyboard.keycode==ALLEGRO_KEY_P) {powieksz(mrowisko,pamiec); }
            if(ev.keyboard.keycode==ALLEGRO_KEY_Z) {zmniejsz(mrowisko,pamiec); }
            if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE) break;

        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            rysujMrowki(mrowisko);
            rysujKwadraty(pamiec);
            al_flip_display();

        }

    }

    while(!done1)
    {
        al_clear_to_color(al_map_rgb(255,255,255));

        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) done1 = true;
        else if(ev.type == ALLEGRO_EVENT_TIMER) {redraw = true;}
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) done1 = true;

                if(ev.keyboard.keycode ==ALLEGRO_KEY_LEFT)
                    {
                        if(FPS > 1)FPS--;
                        else FPS = 1;
                        al_set_timer_speed(timer, 1.0/FPS);
                    }
                if(ev.keyboard.keycode ==ALLEGRO_KEY_RIGHT)
                    {
                        FPS++;
                        al_set_timer_speed(timer, 1.0/FPS);
                    }

                if(ev.keyboard.keycode==ALLEGRO_KEY_P) {powieksz(mrowisko,pamiec); redraw=true; }
                if(ev.keyboard.keycode==ALLEGRO_KEY_Z) {zmniejsz(mrowisko,pamiec); redraw=true; }
                if(ev.keyboard.keycode ==ALLEGRO_KEY_SPACE) //pause
                    {
                       if(al_get_timer_started(timer))
                                al_stop_timer(timer);
                        else
                                al_start_timer(timer);
                    }

                 if(ev.keyboard.keycode ==ALLEGRO_KEY_ENTER && !al_get_timer_started(timer)) //reset
                    {
                                        mrowisko.clear();
                                        pamiec.clear();
                                        redraw = true;
                                        done = false;
                                        goto START;
                    }

            }

        if(redraw && al_is_event_queue_empty(event_queue))
            {
                redraw = false;

               while(!czywyjda(mrowisko))
                {

                for(int i = 0; i < mrowisko.size(); ++i)
                    {
                        zmniejsz(mrowisko,pamiec);

                    }

                }


                  for(int i = 0; i < mrowisko.size(); ++i){
                                mrowisko[i].obrot(pamiec);
                                mrowisko[i].zmienkolor(pamiec);
                                mrowisko[i].krok();
                  }

            rysujKwadraty(pamiec);
            rysujMrowki(mrowisko);
            al_flip_display();
            }

    }





return 0;

}
