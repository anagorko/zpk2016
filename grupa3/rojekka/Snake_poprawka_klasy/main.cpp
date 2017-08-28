#include "snake.h"
#include "pokarm.h"
#include "gra.h"
#include "grafika.h"
#include "menu.h"


//****************************************************************************************************************
int main()
{
    int szerokosc_okna = 1200;          //mozna zrobic, zeby uzytkownik definiowal
    int wysokosc_okna = 800;
    int szerokosc_planszy = 1000;
    int wysokosc_planszy = 800;
    int pozycja_poczatkowa_weza = szerokosc_okna - szerokosc_planszy;

    const char* sciezka_waz = "./Waz.bmp";
    const char* sciezka_pokarm = "./pokarm.bmp";
    const char* sciezka_mapa = "./Mapa.bmp";

    Grafika grafika(sciezka_waz, sciezka_pokarm, sciezka_mapa, szerokosc_okna, wysokosc_okna);
    Menu menu(grafika);

    while(true){                                            // Glówna petla aplikacji
        int wskaz=menu.wyswietlMenuGlowne();                              // wywolanie funkcji menu() i zapamietanie zwroconej przez nia wartosci
        if(wskaz==1){
            Gra gra;
            Snake snake(pozycja_poczatkowa_weza, pozycja_poczatkowa_weza, Snake::W_PRAWO);
            Pokarm pokarm(szerokosc_planszy, wysokosc_planszy);

            gra.startGlownejRozgrywki(grafika, snake, pokarm, menu);
        }
        else if (wskaz==2){                                //jesli menu() zworcilo 2
            menu.wyswietlInformacjeOGrze();
        }
        else if (wskaz==3){                                //jesli menu() zwrocilo 3
            break;                                          //konczymy glowna petle
        }
    }

    return 0;
}
END_OF_MAIN()



