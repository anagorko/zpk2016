#ifndef GRAFIKA_H
#define GRAFIKA_H

#include "snake.h"
#include "pokarm.h"
#include "grafika.h"
#include <allegro.h>


class Grafika{
    int _szerokosc_okna;
    int _wysokosc_okna;
    BITMAP* waz;
    BITMAP* mapa;
    BITMAP* pokarm;
public:
    BITMAP* bufor;
    Grafika(const char* bitmapa_waz, const char* bitmapa_pokarm, const char* bitmapa_mapa, int szerokosc_okna, int wysokosc_okna);
    ~Grafika();
    void wypelnijTlo(int R, int G, int B);
    void wyswietlBufor();
    void wyswietlTekst( const char* tekst, int x, int y, int inverted = -1, int R = 200, int G=200, int B=200);
    void wyswietlPlansze();
    void wyswietlPokarm(Pokarm& jedzenie);
    void wyswietlWeza(Snake& snake);
};

#endif // GRAFIKA_H
