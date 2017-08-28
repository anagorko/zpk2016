#ifndef POKARM_H
#define POKARM_H

#include <ctime>
#include <cstdlib>

#include "snake.h"

class Pokarm
{
    int _szerokosc_planszy;
    int _wysokosc_planszy;
    int _x;
    int _y;
    void wylosujNowaPozycjePokarmu();
public:
    Pokarm(int szerokosc_planszy, int wysokosc_planszy);
    void wygenerujNowyPokarm(const Snake& snake);
    int zwrocPozycjeX() { return _x; }
    int zwrocPozycjeY() { return _y; }
};

#endif // POKARM_H
