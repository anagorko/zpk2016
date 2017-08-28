#ifndef SNAKE_H
#define SNAKE_H

#include <allegro.h>
#include <vector>

class Snake{
    int _x, _y, _kierunek;
    struct ogon{
        int x;
        int y;
    };
public:
    std::vector<ogon> ogony;
    Snake(int pozycja_x, int pozycja_y, int kierunek);
    void uaktualnijPozycjeGlowy();
    void uaktulanijPozycjeOgonow();
    int zwrocPozycjeX() const { return _x; }
    int zwrocPozycjeY() const { return _y; }
    int zwrocKierunek() const { return _kierunek; }
    unsigned int zwrocIloscOgonow() const { return ogony.size(); }
    void zmienKierunekPoruszaniaSie(bool byl_klawisz);
    bool czyNastalaKolizja();
    void dodajJedenOgon();
    enum { W_PRAWO = 0, W_LEWO, W_DOL, W_GORE };

};

#endif // SNAKE_H
