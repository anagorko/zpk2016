#include "pokarm.h"

Pokarm::Pokarm(int szerokosc_planszy, int wysokosc_planszy){
    _szerokosc_planszy = szerokosc_planszy;
    _wysokosc_planszy = wysokosc_planszy;

}

void Pokarm::wylosujNowaPozycjePokarmu(){
    srand(time(NULL));
    _x=(rand()%_szerokosc_planszy)/20;                                         //
    _x=_x*20;                                             //
    _y=(rand()%_wysokosc_planszy)/20;                                          //
    _y=_y*20;
}

void Pokarm::wygenerujNowyPokarm(const Snake& snake){
    bool zle_wylosowany_pokarm;                                                           //*************************************
    do{                                                                     //losujemy nowe wpolrzedne pokarmu
        zle_wylosowany_pokarm=false;                                                      //
        wylosujNowaPozycjePokarmu();
                                                                            //
        if(_x==snake.zwrocPozycjeX() && _y==snake.zwrocPozycjeY())                        //dopoki nie sa to wsp glowy weza
            zle_wylosowany_pokarm=true;                                                   //
                                                                            //
        for(unsigned int i=0; i<snake.zwrocIloscOgonow(); i++){                                  //
            if(_x==snake.ogony[i].x && _y==snake.ogony[i].y){             //albo wsp jego ciala
                zle_wylosowany_pokarm=true;                                               //
            }                                                               //
        }                                                                   //
    }while(zle_wylosowany_pokarm);                                                        //***********************************
}
