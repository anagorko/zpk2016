#include "gra.h"

Gra::Gra(){
    level = 1;
    aktualnie_wykonanych_ruchow = 0;
    score = 0;
    poczatek_gry = time(NULL);
}

void Gra::obliczMinimalnaLiczbeRuchowDoPokarmu(Snake &snake, Pokarm &pokarm){
    int ruchy_x=pokarm.zwrocPozycjeX()-snake.zwrocPozycjeX();       //**************************************
    if(ruchy_x<0){                                              //
        ruchy_x=-ruchy_x;                                       //
    }                                                           //
    if(ruchy_x>500){                                            //obliczanie minimalnej liczby ruchow do pokarmu
        ruchy_x=1000-ruchy_x;                                   //
    }                                                           //
                                                                //
    int ruchy_y=pokarm.zwrocPozycjeY()-snake.zwrocPozycjeY();                                  //
    if(ruchy_y<0){                                              //
        ruchy_y=-ruchy_y;                                       //
    }                                                           //
    if(ruchy_y>500){                                            //
        ruchy_y=1000-ruchy_y;                                   //
    }                                                           //

    minimalna_liczba_ruchow_do_pokarmu=ruchy_x/20 + ruchy_y/20;                //*******************************************
}

void Gra::uaktualnijPoziomGry(Snake& snake){
    level = (snake.zwrocIloscOgonow()/5)+1;                       //obliczamy poziom na podstawie dlugosci
}

void Gra::uaktualnijLiczbePunktow(){
    //obliczanie punktow za zdobyty pokarm
    int score_tmp=2*level*100-((aktualnie_wykonanych_ruchow-minimalna_liczba_ruchow_do_pokarmu)*10*level);
    if(score_tmp<level*100)
        score_tmp=level*100;
    //dodajemy do calkowitego wyniku, wynik tymczasowy
    score += score_tmp;
}

void Gra::czekajNaWykonanieRuchu(Snake& snake){
    int ticks=clock();                                                              //tak jak wczesniej tiki zegara do obliczenia czasu
    bool byl_klawisz=false;                     //flaga zapobiegajaca kikukrotnym wcisniecia klawisza
    while((double)(clock()-ticks)/CLOCKS_PER_SEC<wspolczynnik_predkosci/level){    //czas odswiezania symulacji maleje wraz z levelem co za tym idzie im wyzszy level tym szybciej sie porusza
        if (!byl_klawisz)
            snake.zmienKierunekPoruszaniaSie(byl_klawisz);
    }
}

int Gra::startGlownejRozgrywki(Grafika& grafika, Snake& snake, Pokarm& pokarm, Menu& menu){

    pokarm.wygenerujNowyPokarm(snake);
    obliczMinimalnaLiczbeRuchowDoPokarmu(snake, pokarm);

    while( !key[ KEY_ESC ] ){

        czekajNaWykonanieRuchu(snake);
        snake.uaktualnijPozycjeGlowy();
        zwiekszLiczbeAktualnychRuchow();

        if(snake.zwrocPozycjeX()==pokarm.zwrocPozycjeX() && snake.zwrocPozycjeY()==pokarm.zwrocPozycjeY()){       //jesli glowa weza jest na polu z pokarmem

            snake.dodajJedenOgon();
            uaktualnijLiczbePunktow();
            pokarm.wygenerujNowyPokarm(snake);
            obliczMinimalnaLiczbeRuchowDoPokarmu(snake, pokarm);
            wyzerujAktualnaLiczbeRuchow();

        }
        else{                                                          //jesli nie udalo sie zdobyc pokarmu
            if(snake.zwrocIloscOgonow()>=2){                                        //jesli dlugosc weza jest wieksza od 2
               if ( snake.czyNastalaKolizja() ){
                    menu.wyswietlEkranKoncowy(score);
                   break;
               }
            }
            if(snake.zwrocIloscOgonow() != 0){                                 //jesli istnieja jakies czesci weza
                snake.uaktulanijPozycjeOgonow();
            }
        }

        uaktualnijPoziomGry(snake);

        grafika.wypelnijTlo(134,78,100);
        grafika.wyswietlPlansze();
        grafika.wyswietlPokarm(pokarm);
        grafika.wyswietlWeza(snake);

        wyswietlPunkty(grafika);

        grafika.wyswietlBufor();
    }

    return 0;

}

void Gra::wyswietlPunkty(Grafika& grafika){
    textprintf_ex( grafika.bufor, font, 1015, 20, makecol( 200, 200, 200 ), - 1, "Poziom: %d !", zwrocPoziomGry() );                       //dajemy informacje o poziomie
    textprintf_ex( grafika.bufor, font, 1015, 40, makecol( 200, 200, 200 ), - 1, "Liczba punktow: %d !", zwrocLiczbePunktow() );               //liczbie pkt
    textprintf_ex( grafika.bufor, font, 1015, 60, makecol( 200, 200, 200 ), - 1, "Czas: %lds !", zwrocCzasOdPoczatkuGry() );          //i czasie gry
}



