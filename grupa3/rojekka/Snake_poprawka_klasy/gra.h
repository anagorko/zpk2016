#ifndef GRA_H
#define GRA_H

#include <ctime>

#include "snake.h"
#include "pokarm.h"
#include "grafika.h"
#include "menu.h"

class Gra{
    int level;
    int score;
    int minimalna_liczba_ruchow_do_pokarmu;
    int aktualnie_wykonanych_ruchow;
    static const double wspolczynnik_predkosci = 0.5;
    time_t poczatek_gry;
    void wyswietlPunkty(Grafika& grafika);
public:
    Gra();
    void obliczMinimalnaLiczbeRuchowDoPokarmu(Snake& snake, Pokarm& pokarm);
    void zwiekszLiczbeAktualnychRuchow() { aktualnie_wykonanych_ruchow++; }
    void wyzerujAktualnaLiczbeRuchow() { aktualnie_wykonanych_ruchow = 0; }
    void uaktualnijPoziomGry(Snake& snake);
    void uaktualnijLiczbePunktow();
    int zwrocLiczbePunktow() { return score; }
    int zwrocPoziomGry() { return level; }
    void czekajNaWykonanieRuchu(Snake& snake);
    long int zwrocCzasOdPoczatkuGry() { return time(NULL) - poczatek_gry; }
    int startGlownejRozgrywki(Grafika& grafika, Snake& snake, Pokarm& pokarm, Menu& menu);
};

#endif // GRA_H
