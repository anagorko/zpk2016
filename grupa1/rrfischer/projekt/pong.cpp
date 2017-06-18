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


class Paletka
{
public:
    float x1,x2;
    float y1,y2;
    Paletka(float);
    ~Paletka() {};
    void ruchG (int dy=2);
    void ruchD (int dy=2);
    void rysuj(float x11);
    void ustaw_wysokosc(float y);

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


int main()
{
    const int FPS=60;

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

    float z_1=SZER/2-7.5-2.5,z_2=WYS/2-7.5;  //wspolrzedne pilki, -2.5 bo pasek na bitmapie jest przesuniety o 2.5 od srodka
    float z_12=z_1+15, z_22=z_2+15;
    float dx=0, dy=0;    //predkosc pilki
    int wynik1=0,wynik2=0;
    bool restart = true;
    bool restart2 = false;
    int czas_restart [2]= {FPS*2,FPS*2};
    float max_roznica=55;   //40+15 (polowa paletki +pilka)
    float kat_odbicia=40;

    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT *font = al_load_font("font.ttf",35,0);
    ALLEGRO_FONT *font2 = al_load_font("font.ttf",20,0);
    //info o bledach
    if (!font||!font2||!al_init()||!okno||!tlo)
        return -1;

    Paletka paletka1 (25);
    Paletka paletka2 (SZER-40);


    do
    {

        al_draw_scaled_bitmap(tlo,0,0,714,420,0,0,SZER,WYS,0);  //tlo dopasowuje sie do wielkosci ekranu
        al_get_keyboard_state(&klawiatura);
        paletka1.rysuj(25);
        paletka2.rysuj(SZER-40);

        if (al_key_down(&klawiatura, ALLEGRO_KEY_S)  && paletka1.y2 < WYS)  paletka1.ruchD();
        if (al_key_down(&klawiatura, ALLEGRO_KEY_W)    && paletka1.y1 > 0)  paletka1.ruchG();
        if (al_key_down(&klawiatura, ALLEGRO_KEY_DOWN)  && paletka2.y2 < WYS)  paletka2.ruchD();
        if (al_key_down(&klawiatura, ALLEGRO_KEY_UP)    && paletka2.y1 > 0)    paletka2.ruchG();

        al_draw_filled_rectangle(z_1, z_2, z_12, z_22,al_map_rgb_f(1.0, 1.0, 1.0)) ;       // rysowanie pi³ki





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

//Update pozycji
        z_1+=dx;
        z_12+=dx;
        z_2+=dy;
        z_22+=dy;

//kolizje sciany gora- dol
        if(z_2<0) dy=2;
        else if (z_22>WYS) dy=-2;

//kolizja PALETKI

        else if ((z_1<40) && (z_22>=paletka1.y1) && (z_2<=paletka1.y2) && (z_1>paletka1.x1))
        {
//kierunek odbicia pilki od paletki 1, w zaleznosci od miejsca trafienia pilki to inny kat odbicia
            float srodek_paletki1=(paletka1.y2-paletka1.y1)/2;
            float srodek_pilki=(z_22-z_2)/2;
            float roznica=abs(srodek_paletki1-srodek_pilki);
            float kat = (roznica/max_roznica)*kat_odbicia;
            float v=sqrt(pow(dx,2)+pow(dy,2));
            dx=v*cos((M_PI/180)*kat); //zamiana kata w stopniach na radiany
//dy=v*-sin((M_PI/180)*kat);

        }

        else if ((z_12>SZER-40) && (z_22>=paletka2.y1) && (z_2<=paletka2.y2) && (z_12<paletka2.x2))
        {

//kierunek odbicia pilki od paletki 2, w zaleznosci od miejsca trafienia pilki to inny kat odbicia
            float srodek_pilki=(z_22-z_2)/2;
            float srodek_paletki2=(paletka2.y2-paletka2.y1)/2;
            float roznica2=abs(srodek_pilki-srodek_paletki2);
            float kat2 = (roznica2/max_roznica)*kat_odbicia;
            float v=sqrt(pow(dx,2)+pow(dy,2));
            dx=v*-cos((M_PI/180)*kat2);
//dy=v*-sin((M_PI/180)*kat2);  */

        }

        //RESTART gry
        else if (z_12+dx<0)
        {
            wynik2++;
            z_1=SZER/2-7.5-2.5;   //ustawienie pilki na srodku ekranu
            z_2=WYS/2-7.5;
            z_12=z_1+15;
            z_22=z_2+15;
            dx=0;
            dy=0;
            restart2=true;

        }


        else if (z_1+dx>SZER)
        {
            wynik1++;
            z_1=SZER/2-7.5-2.5;
            z_2=WYS/2-7.5;
            z_12=z_1+15;
            z_22=z_2+15;
            dx=0;
            dy=0;
            restart=true;
        }
//alternatywna gra z końcem do 10

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
