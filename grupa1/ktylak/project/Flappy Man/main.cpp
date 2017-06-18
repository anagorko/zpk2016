
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include <cstdlib>
#include <cmath>
#include "man.h"
#include "pipe.h"


using namespace std;

bool done = false;

//ROZMIAR OKIENKA
const int screen_width = 900;
const int screen_height = 473;
const float FPS = 60;

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_BITMAP *background;
ALLEGRO_BITMAP *imageman;
ALLEGRO_BITMAP *imagePipe;
ALLEGRO_BITMAP *gameover;
ALLEGRO_FONT   *tekst;
ALLEGRO_TIMER  *timer = NULL;



void init_allegro(){
    display = NULL;
    display = NULL;
    al_init();
    al_init_primitives_addon();

    display = al_create_display(screen_width, screen_height);

    // ZAINICJOWANIE KOLEJKI ZDARZEN KLAWIARUTY
    al_install_keyboard();
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // ZAINICJOWANIE TIMERA - PREDKOSC GRY
    timer = al_create_timer(0.03);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    // ZAINICJOWANIE CZCIONKI
    al_init_font_addon();
    al_init_ttf_addon();
    tekst = al_load_font("arial.ttf", 20, 0);

}

class GamePlay{
    int manX = 40;
    int manY = 500 / 2;
    int manWidth = 0;
    int manHeight = 0;
    int manWidthDisp = 60;
    int manHeightDisp = 38;
    int pipeWidth = 0;
    int pipeHeight = 0;
    int pipeX = 300;
    int pipeDist = 200;
    int pipeHole = 200;


public:
    bool error();
    void destroy();
    bool game();
};


bool GamePlay::game(){

    //srand(time(NULL));

    Pipe pipe(pipeWidth,pipeHeight,pipeX,pipeDist,pipeHole);
    Man man(manX, manY, manWidth, manHeight, manWidthDisp, manHeightDisp);

// LOS - ZMIENA LOSUJACA WYSOKOSC RYSOWANIA LEWEGO GORNEGO ROGU RURY
    int los[1000];
    for(int i=0; i<1000; i++)
            los[i] =  -4*(rand()%100);

    int wynik=0;          //ZMIENNA PRZECHOWUJACA WYNIK
    error();              // ZWRACA INFORMACJE O BLEDZIE

// ZALADOWANIE PLIKOW Z GRAFIKAMI I PRZYPISANIE IM NAZW
	background = al_load_bitmap("images/background5.png");
    imageman = al_load_bitmap("images/bat.png");
    imagePipe = al_load_bitmap("images/pipe.png");
    pipeWidth =  al_get_bitmap_width(imagePipe);
	pipeHeight = al_get_bitmap_height(imagePipe);
	manWidth = al_get_bitmap_width(imageman);
	manHeight = al_get_bitmap_width(imageman);
	gameover = al_load_bitmap("images/gameover.png");

//WSTAWIENIE NAPISU NA POCZATKU
   /* al_draw_scaled_bitmap(background,0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, 0);
    al_draw_textf(tekst, al_map_rgb(255, 255, 255), 380, 270, ALLEGRO_ALIGN_LEFT, "Get ready!", wynik);
    al_flip_display();
    al_rest(2); */


	while(!done)
	{

	 	ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

       if(ev.type == ALLEGRO_EVENT_TIMER) //INICJALIZACJA GRY - JESLI GRA SIE ZACZNIE TO...
        {
            manY += 1;       // POSTAC SPADA O 1 JESLI NIE MA ZADNEGO RUCHU
            pipeX -= 2;      // RURY PRZESUWAJA SIE O 2

            for(int n=0; n<1000; n++)  // W TYM PRZYPADKU WYBRANA LICZNA RUR TO 1000, A WIEC MOZE TYLE PUNKTOW MAX ZDOBYC, STAD OGRANICZENIE PETLI
            {
                if (manX==pipeX+(pipeDist*n))    //PUNKT ZA POKONANA RURE, PipeDist OZNACZA SZER MIEDZY RURAMI, A WIEC Z KAZDA KOLEJNA RURA POZYCJA X POSTACI ZWIEKSZA SIE O DANA LICZBE PIKSLEI CZYLI STAD MNOZENIE
                                                 //JESLI PTAK ZNAJDUJE SIE NA POZYCJI PRZYNAJMNIEJ SZEROKOSC RURY+PipeDist TO ZNACZY ZE MINAL RURE I ZDOBYL PUNKT
                wynik++;
            }

             if (manY>=screen_height - manHeightDisp+10)    // UDERZENIE W ZIEMIE - KONIEC GRY (+10 W CELU LEPSZEGO DOPASOWANIA)
             {                                              // POZYCJA PTAKA (DOL OKNA- WYSOKOSC PTAKA) - SPADŁ ==> KONIEC GRY
				break;
             }

        }

		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
            case ALLEGRO_KEY_UP:
				manY -= 40;
                al_draw_bitmap(imageman, manX, manY, 0);

        // POSTAC MOZE ISC NA DOL
            case ALLEGRO_KEY_DOWN:
				manY += 10;
                al_draw_bitmap(imageman, manX, manY, 0);

			}

		}

		// RYSOWANIE PLANSZY
		al_draw_scaled_bitmap(background,0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, 0);

		//RYSOWANIE POSTACI
		al_draw_scaled_bitmap(imageman,0,0, manWidth, manHeight,manX, manY, manWidthDisp, manHeightDisp, 0);

//PETLA ODPOWIADAJACA ZA RYSOWANIE RUR

        for(int n=0; n<1000; n++)
        {
            //POLECENIE RYSUJACE
           al_draw_scaled_bitmap(imagePipe, 0, 0, pipeWidth, pipeHeight,pipeX+(pipeDist*n), los[n], pipeWidth, pipeHeight, 0);
            //ROTATED - ODWRACAMY OBRAZEK I RURY SA WIDOCZNE Z GORY I Z DOLU
           al_draw_scaled_rotated_bitmap(imagePipe, 0, 0, pipeX+(pipeDist*n), los[n] + 2*pipeHeight + pipeHole, 1,1, M_PI , 0);

                //WARUNEK X UDERZENIA W RURE
           if((manX>pipeX+(pipeDist*n) && manX<pipeX+(pipeDist*n)+pipeWidth) || (manX+manWidthDisp+20>pipeX+(pipeDist*n) && manX+manWidthDisp+20<pipeX+(pipeDist*n)+pipeWidth))
                 {
                     //WARUNEK  Y UDERZENIA W RURE
                 if(manY >= los[n]+pipeHeight+pipeHole-manHeightDisp+10|| manY <=los[n]+pipeHeight)
                     {
                        done = true;
                        break;

                     }
                 }

        }
        //WYSWIETLANIE AKTUALNEGO WYNIKU GRY (FONT, KOLOR< POZYCJA, TRESC)
        al_draw_textf(tekst, al_map_rgb(255, 255, 255), 0, 2, ALLEGRO_ALIGN_LEFT, "TWÓJ WYNIK = %i", wynik);

        al_flip_display();

    }

// WYSWIETLENIE KONCOWEGO NAPISU GAME OVER
    al_draw_bitmap(gameover,350,200,0);
    al_draw_textf(tekst, al_map_rgb(255, 255, 255), 380, 270, ALLEGRO_ALIGN_LEFT, "TWÓJ WYNIK TO %i", wynik);
    al_flip_display();
    al_rest(3);

//USUNIECIE OBIEKTOW I ZWOLNIENIE PAMIECI
    destroy();

	return 0;
}

bool GamePlay::error(){
	 if(!al_init()) {
        cerr << "Blad podczas inicjalizacji allegro." << endl;
        return -1;
    }

    if (!al_init_primitives_addon()) {
        cerr << "Blad podczas inicjalizacji dodatku 'primitives'." << endl;
        return -1;
    }

    if(!al_init_image_addon()) {
        cerr << "Blad podczas inicjalizacji dodatku 'image'." << endl;
        return -1;
    }

    if(!al_install_keyboard()) {
        cerr << "Blad podczas inicjalizacji klawiatury." << endl;
        return -1;
    }

    if(!timer) {
        cerr << "Blad podczas inicjalizacji zegara." << endl;
        return -1;
    }

    if(!display) {
        cerr << "Blad podczas inicjalizacji ekranu." << endl;
        al_destroy_timer(timer);
        return -1;
    }

    if(!event_queue) {
        cerr << "Blad podczas inicjalizacji kolejki zdarzeñ." << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
}

void GamePlay::destroy(){

    //USUNIECIE OBIEKTOW
    al_destroy_font(tekst);
    al_destroy_bitmap(imagePipe);
    al_destroy_bitmap(imageman);
    al_destroy_bitmap(background);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
}
int main(){
    //WYWOLANIE METODY init_allegro
    init_allegro();

    //TWORZENIE OBIEKTU KLASY GamePlay
    GamePlay gameplay;

    //WYWOLANIE METODY game() KLASY GamePlay
    gameplay.game();
}
