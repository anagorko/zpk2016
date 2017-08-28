#include "snake.h"

Snake::Snake(int pozycja_x, int pozycja_y, int kierunek){
    _x = pozycja_x;
    _y = pozycja_y;
    _kierunek = kierunek;
    ogony = {};
}

void Snake::uaktualnijPozycjeGlowy(){
    if(_kierunek==W_PRAWO){                   //jesli waz skierowany w prawo
        _x+=20;                    //dodajemy do x
        if(_x>980)                 //jesli przeszlismy poza plansze
            _x=0;                  //pojawiamy sie z drugiej strony
    }
    else if(_kierunek==W_LEWO){              //jesli waz skierowany w lewo
        _x-=20;                    //odejmujemy od x
         if(_x<0)
            _x=980;
    }
    else if(_kierunek==W_DOL){              //jesli waz skierowany w dol
        _y+=20;                    //dodajemy do y
         if(_y>780)
            _y=0;
    }
    else if(_kierunek==W_GORE){              //jesli waz skierowany w gore
        _y-=20;                    //odejmujemy od y
         if(_y<0)
            _y=780;
    }
}

void Snake::uaktulanijPozycjeOgonow(){
    ogony[ogony.size()-1].x=_x;                //to ostatni kawalek przeskakuje w miejsce glowy w poprzednim ruchu
    ogony[ogony.size()-1].y=_y;
    if(_kierunek==W_PRAWO){
        ogony[ogony.size()-1].x-=20;
        if(ogony[ogony.size()-1].x<0)
            ogony[ogony.size()-1].x=980;
    }
    else if(_kierunek==W_LEWO){
        ogony[ogony.size()-1].x+=20;
        if(ogony[ogony.size()-1].x>980)
            ogony[ogony.size()-1].x=0;
    }
    else if(_kierunek==W_DOL){
        ogony[ogony.size()-1].y-=20;
        if(ogony[ogony.size()-1].y<0)
            ogony[ogony.size()-1].y=780;
    }
    else if(_kierunek==W_GORE){
        ogony[ogony.size()-1].y+=20;
        if(ogony[ogony.size()-1].y>780)
            ogony[ogony.size()-1].y=0;
    }
}

void Snake::zmienKierunekPoruszaniaSie(bool byl_klawisz){
        if( key[ KEY_LEFT ] ) {
            if(_kierunek != W_PRAWO && byl_klawisz==false){
                byl_klawisz=true;
                _kierunek=W_LEWO;
            }
        }            //

        if( key[ KEY_RIGHT ] ) {
            if(_kierunek!=W_LEWO && byl_klawisz==false) {
                byl_klawisz=true;
                _kierunek=W_PRAWO;
            }
        }           //

        if( key[ KEY_UP ] ) {
            if(_kierunek!=W_DOL && byl_klawisz==false) {
                byl_klawisz=true;
                _kierunek=W_GORE;
            }
        }              //

        if( key[ KEY_DOWN ] ) {
            if(_kierunek!=W_GORE && byl_klawisz==false)  {
                byl_klawisz=true;
                _kierunek=W_DOL;
            }
        }
}

bool Snake::czyNastalaKolizja(){
    for(unsigned int i=0; i<ogony.size()-1; i++){                    //dla kazdej czasci weza
        ogony[i]=ogony[i+1];                                //przesuwamy weza (zapominajac o ostatnim kawalku ktorym zajmiemy sie nizej)
        if(ogony[i].x==_x && ogony[i].y==_y){       //sprawdzamy czy nie koliduje z glowa
            return true;
        }
    }
    return false;
}

void Snake::dodajJedenOgon(){
    ogon o;                                          //dodajemy nowy segment ciala
    o.x=_x;                              //poczotkowo nadajemy wspolrzedne glowy weza
    o.y=_y;
    if(_kierunek==W_PRAWO){                             //i odpowiednio modyfikujemy w zaleznosci w jakiej pozycji weza zostal zjedzony pokarm
        o.x-=20;
        if(o.x<0)
            o.x=980;
    }
    else if(_kierunek==W_LEWO){
        o.x+=20;
        if(o.x>980)
            o.x=0;
    }
    else if(_kierunek==W_DOL){
        o.y-=20;
        if(o.y<0)
            o.y=780;
    }
    else if(_kierunek==W_GORE){
        o.y+=20;
        if(o.y>780)
            o.y=0;
    }

    ogony.push_back(o);                     //dodajmy nowa czesc do naszej listy
}

