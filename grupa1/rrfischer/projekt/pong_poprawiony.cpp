#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <iomanip>
using namespace std;

const int SZER = 700;
const int WYS = 420;
const int FPS=60;
int czas_restart [2]= {FPS*2,FPS*2};
float dx=0, dy=0;    //predkosc pilki
float max_roznica=47.5;   //40+7.5 (polowa paletki + polowa  pilka)
float kat_odbicia=30;


class Paletka
{
    float x1,x2;
    float y1,y2;
public:
    Paletka(float);
    ~Paletka() {};
    void ruchG (int dy=2);
    void ruchD (int dy=2);
    void rysuj(float x11);
    void ustaw_wysokosc(float y);
    float getx1 (){return x1;}
    float getx2 (){return x2;}
    float gety1 (){return y1;}
    float gety2 (){return y2;}

};

Paletka::Paletka(float x11)
{
    x1=x11;
    y1=WYS/2-40;
    x2=x11+15;
    y2=WYS/2+40;
}

void Paletka::rysuj(float x11)
{
    al_draw_filled_rectangle(x1, y1, x2, y2,al_map_rgb_f(1.0, 1.0, 1.0));
}

void Paletka::ustaw_wysokosc(float y)
{
    y1=y-40;
    y2=y+40;
}
void Paletka::ruchG (int dy)
{
    y1-=dy;
    y2-=dy;
}
void Paletka::ruchD (int dy)
{
    y1+=dy;
    y2+=dy;
}

class Pilka
{
    float x1, x2, y1, y2;
public:
    Pilka(float, float);
    ~Pilka() {};
    void ustaw(float, float);
    void rysuj();
    void update();
    void kolizja_sciana();
    float getx1 (){return x1;}
    float getx2 (){return x2;}
    float kolizja_paletka(Paletka p, Pilka pil);
};
Pilka::Pilka(float x11, float y11)
{
    x1=SZER/2-7.5-2.5;
    y1=WYS/2-7.5;  //wspolrzedne pilki, -2.5 bo pasek na bitmapie jest przesuniety o 2.5 od srodka
    x2=x1+15;
    y2=y1+15;
}

void Pilka::rysuj()
{
    al_draw_filled_rectangle(x1, y1, x2, y2,al_map_rgb_f(1.0, 1.0, 1.0)) ;
}

void Pilka::ustaw(float x, float y)
{
    x1=x-7.5-2.5;
    x2=x+7.5-2.5;
    y1=y-7.5;
    y2=y+7.5;
}
void Pilka::update()
{
    x1+=dx;
    x2+=dx;
    y1+=dy;
    y2+=dy;
}
void Pilka::kolizja_sciana()
{
    if(y1<0) dy=2;
    else if (y2>WYS) dy=-2;
}

float Pilka::kolizja_paletka(Paletka p, Pilka pil)
{
    float srodek_paletki=(p.gety2()-p.gety1())/2;
    float srodek_pilki=(pil.y2-pil.y1)/2;
    float roznica=abs(srodek_paletki-srodek_pilki);
    float kat= (roznica/max_roznica)*kat_odbicia;
    float v=sqrt( pow(dx,2)+pow(dy,2));
    if ((pil.x2>SZER-40) && (pil.y2>=p.gety1()) && (pil.y1<=p.gety2()) && (pil.x2<p.getx2()))
        return  dx=v*-cos((M_PI/180)*kat);
    else if ((pil.x1<40) && (pil.y2>=p.gety1()) && (pil.y1<=p.gety2()) && (pil.x1>p.getx1()))
        return  dx=v*cos((M_PI/180)*kat);
    else return 0;
}


int main()
{

    al_init();
    ALLEGRO_DISPLAY *okno= al_create_display(SZER, WYS);

    al_init_primitives_addon();
    al_set_window_title(okno, "PONG");
    al_install_keyboard();
    ALLEGRO_KEYBOARD_STATE klawiatura;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS); //60 FPS, po każdej 1/60 sec gra sie updatuje
    al_start_timer(timer);
//tlo bitmapa
    al_init_image_addon();
    ALLEGRO_BITMAP *tlo=NULL;
    tlo=al_load_bitmap("tlo2.bmp");
    int wynik1=0,wynik2=0;
    bool restart = true;
    bool restart2 = false;

    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT *font = al_load_font("font.ttf",35,0);
    ALLEGRO_FONT *font2 = al_load_font("font.ttf",20,0);
    //info o bledach
    if (!font||!font2||!al_init()||!okno||!tlo)
        return -1;


    Paletka paletka1 (25);
    Paletka paletka2 (SZER-40);
    Pilka pileczka (SZER/2-7.5-2.5,WYS/2-7.5);

    do
    {

        al_draw_scaled_bitmap(tlo,0,0,714,420,0,0,SZER,WYS,0);  //tlo dopasowuje sie do wielkosci ekranu
        al_get_keyboard_state(&klawiatura);
        paletka1.rysuj(25);
        paletka2.rysuj(SZER-40);
        pileczka.rysuj();
        if (al_key_down(&klawiatura, ALLEGRO_KEY_S)  && paletka1.gety2() < WYS)  paletka1.ruchD();
        if (al_key_down(&klawiatura, ALLEGRO_KEY_W)    && paletka1.gety1() > 0)  paletka1.ruchG();
        if (al_key_down(&klawiatura, ALLEGRO_KEY_DOWN)  && paletka2.gety2() < WYS)  paletka2.ruchD();
        if (al_key_down(&klawiatura, ALLEGRO_KEY_UP)    && paletka2.gety1() > 0)    paletka2.ruchG();


//dla pilki lecacej w prawo zatrzymanie czasu wyrzutu pilki, dodatnia predkosc//serw w prawo
        if (restart)
        {
            //zmniejsza czas do restartu
            czas_restart[0] --;
            if (czas_restart[0]==0)

            {
                restart=false;
                czas_restart[0]=czas_restart[1];

                dx=2; //serw w prawo
                dy=2;
            }
        }


//dla pilki lecacej w lewo zatrzymanie czasu wyrzutu pilki,ujemna predkosc// serw w lewo
        if (restart2==true)
        {
            //zmniejsza czas do restartu
            czas_restart[0] --;
            if (czas_restart[0]==0)

            {
                restart2=false;
                czas_restart[0]=czas_restart[1];

                dx=-2; //serw w lewo
                dy=2;
            }
        }
//RUCH PILKI

        pileczka.update();
        pileczka.kolizja_sciana();
        pileczka.kolizja_paletka(paletka1, pileczka);
        pileczka.kolizja_paletka(paletka2, pileczka);


        //RESTART gry
        if (pileczka.getx2()+dx<0)
        {
            wynik2++;
            pileczka.ustaw(SZER/2, WYS/2);
            dx=0;
            dy=0;
            restart2=true;

        }


        else if (pileczka.getx1()+dx>SZER)
        {
            wynik1++;
            pileczka.ustaw(SZER/2, WYS/2);
            dx=0;
            dy=0;
            restart=true;
        }


        if(wynik1==1)
        {
            {
                al_draw_textf(font2,al_map_rgb(255,255,255),SZER/2-5,WYS/2-70,ALLEGRO_ALIGN_CENTER,"WYGRYWA GRACZ 1!"); //-5 w celu estetycznym
                restart=false;
            }
//nowa gra
            if (al_key_down(&klawiatura, ALLEGRO_KEY_SPACE))
            {
                dx=2;
                dy=2;
                wynik1=0;
                wynik2=0;
                //ustawienie paletki na srodku gdy wznawia sie gre
                paletka1.ustaw_wysokosc(WYS/2);
                paletka2.ustaw_wysokosc(WYS/2);
            }
        }

        if(wynik2==1)
        {
            {
                al_draw_textf(font2,al_map_rgb(255,255,255),SZER/2-5,WYS/2-70,ALLEGRO_ALIGN_CENTER,"WYGRYWA GRACZ 2!");
                restart2=false;
            }

            if (al_key_down(&klawiatura, ALLEGRO_KEY_SPACE))
            {
                dx=2;
                dy=2;
                wynik1=0;
                wynik2=0;
                paletka1.ustaw_wysokosc(WYS/2);
                paletka2.ustaw_wysokosc(WYS/2);
            }
        }


        al_draw_textf(font,al_map_rgb(255,255,255),SZER/2+28,40,ALLEGRO_ALIGN_CENTER,"%i",wynik2);
        al_draw_textf(font,al_map_rgb(255,255,255),SZER/2-28,40,ALLEGRO_ALIGN_CENTER,"%i",wynik1);

        al_rest(0.01);

        al_flip_display();

    }
    while(!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE));

    al_destroy_display(okno);
    al_destroy_timer(timer);
    al_destroy_font(font);
    al_destroy_font(font2);
    al_destroy_bitmap(tlo);
}
