#ifndef MENU_H
#define MENU_H

#include <allegro.h>
#include "grafika.h"

class Menu
{
    Grafika* grafika;
public:
    Menu(Grafika& grafika);
    int wyswietlMenuGlowne();
    void wyswietlInformacjeOGrze();
    void wyswietlEkranKoncowy(int punkty);
};

#endif // MENU_H
