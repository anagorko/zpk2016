#include "menu.h"

Menu::Menu(Grafika& grafik_in){
    grafika = &grafik_in;
}


int Menu::wyswietlMenuGlowne(){

    int wskaz=1;                            //zmienna przechowujaca wybor opcji z menu
    bool warunek_menu=true;                 //zmienna logiczna mowiaca o tym czy menu powinno byc jeszcze wyswietlane
    bool bylo_przesuniecie=false;           //zmienna logiczna zapobiegajaca zbyt szybkiej zmianie opcji w menu

    while(warunek_menu){

        int ticks=clock();                      //zapietujemy liczbe tikow zegara

        while( (double)(clock()-ticks)/CLOCKS_PER_SEC<0.6){     //wykonuj petle dopoki czas nie przekroczy 0.6s

            if(key[KEY_UP] && !bylo_przesuniecie){                 //jesli strzalak w gore
                wskaz--;                                        //przesuwamy wskaz w gore
                bylo_przesuniecie=true;
                if (wskaz<1)                                    //jesli doszlismy na sama gore
                    wskaz=3;                                    //zaczynamy od dolu
            }
            if(key[KEY_DOWN] && !bylo_przesuniecie){               //jesli strzalka w dol
                wskaz++;                                        //przesuwamy wskaz w dol
                bylo_przesuniecie=true;
                if(wskaz>3)                                     //jesli doszlismy na sam dol
                    wskaz=1;                                    //zaczynamy od gory
            }
            if(key[KEY_ENTER] || key[KEY_SPACE])   {               //jesli eneter lub spacja
                warunek_menu=false;                             //konczymy menu
                bylo_przesuniecie=true;
            }
        }

        bylo_przesuniecie=false;

        int opcja_1 = 1;
        int opcja_2 = -1;
        int opcja_3 = -1;

        if(wskaz==1){                                               //w zaleznosci od wskazu wyswietlamy menu z innym zaznaczeniem
            opcja_1 = 1;
            opcja_2 = -1;
            opcja_3 = -1;
        }

        else if(wskaz==2){
            opcja_1 = -1;
            opcja_2 = 1;
            opcja_3 = -1;
        }

        else if(wskaz==3){
            opcja_1 = -1;
            opcja_2 = -1;
            opcja_3 = 1;
        }

        grafika->wypelnijTlo(157, 146, 001);
        grafika->wyswietlTekst("ROZPOCZNIJ GRE ! ", 500, 20, opcja_1);
        grafika->wyswietlTekst("O GRZE !", 500, 40, opcja_2);
        grafika->wyswietlTekst("WYJSCIE !", 500, 60, opcja_3);
        grafika->wyswietlBufor();
    }
    return wskaz;
}

void Menu::wyswietlInformacjeOGrze(){
    grafika->wypelnijTlo(87,166,57);
    grafika->wyswietlTekst("Klasyczna gra SNAKE. Zbieraj pozywienie, uwazajac przy tym, aby nie potracic wlasnego ogona.", 75, 80);
    grafika->wyswietlTekst("Im dluzszy bedzie waz tym wyzszy poziom, a co za tym idzie wzrasta predkosc rozgrywki." , 75, 120);
    grafika->wyswietlTekst("Za każdy zebrany pokarm dostaniesz okreslona liczbe punktow obliczana na podstawie obecnego poziomu oraz tego jak szybko Ci sie to udalo!", 75, 160);
    grafika->wyswietlTekst("Aby powrocic nacisnij ESC", 75, 200);
    grafika->wyswietlBufor();

   while(!key[KEY_ESC]);
}

void Menu::wyswietlEkranKoncowy(int punkty){
    grafika->wypelnijTlo(55,22,150);
    grafika->wyswietlTekst("PRZEGRALES!!! :(", 300, 100);                                                                   //jesli wystepuje kolizja wyswietlamy ekran konczacy gre
    textprintf_ex( grafika->bufor, font, 300, 130, makecol( 200, 200, 200 ), - 1, "Twoja liczba zdobytych punktow to: %d !", punkty );
    grafika->wyswietlTekst("Aby kontynuowac nacisnij ESC", 300, 160);
    grafika->wyswietlBufor();
    while(!key[KEY_ESC]);
}
