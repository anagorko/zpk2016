#include <allegro.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>

typedef struct {        //struktura przechowujaca wspolrzedne kratki
int x;
int y;
} wsp;

int game(BITMAP *bufor);            //glowna funkcja obslugujaca rozgrywke
int menu(BITMAP *bufor);            //funkcja wyswietlajaca menu
int o_grze(BITMAP *bufor);          //funkcja wyswietlajaca informacje o grze


//****************************************************************************************************************
int main()
{
    allegro_init();                     //zainicjowanie biblioteki allegro
    install_keyboard();                 //aktywowanie klawiatury
    set_color_depth( 16 );              //ustawienie glebi kolorow na 16 bitow
    set_gfx_mode( GFX_AUTODETECT_WINDOWED, 1200, 800, 0, 0 );       //utworzenie okna o rozmiarze 1200x800
    clear_to_color( screen, makecol( 128, 128, 128 ) );             //wyczyszczenie tla

    BITMAP * bufor=NULL;                                            //utworzenie wskaünika na bitmape naszego bufora
    bufor = create_bitmap( 1200, 800 );                             //stworzenie bitmapy bufora o wielkosci naszego okna
    if( !bufor ){                                               //jesli sie nie udalo
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "Nie moge utworzyc bufora !" );
        allegro_exit();                                         //zamykamy allegro
        return 0;                                               //i konczymy dizlanie aplikacji
    }


    while(true){                                            // GlÛwna petla aplikacji
        int wskaz=menu(bufor);                              // wywolanie funkcji menu() i zapamietanie zwroconej przez nia wartosci
        if(wskaz==1){                                       //jesli menu() zwrocilo 1 startujemy gre
            if(game(bufor)){                                //jesli gra zwroci jakis blad
                destroy_bitmap(bufor);
                allegro_exit();                             //zamykamy allegro
                return 0;                                   //i konczymy dzialanie aplikacji
            }
        }else if (wskaz==2){                                //jesli menu() zworcilo 2
            o_grze(bufor);                                  //wyswietlamy info o grze
        }else if (wskaz==3){                                //jesli menu() zwrocilo 3
            break;                                          //konczymy glowna petle
        }
    }

destroy_bitmap(bufor);                                  //po skonczeniu glownej petli sprzatamy po sobie i konczymy
allegro_exit();
return 0;
}
END_OF_MAIN();


//********************************************************************************************************************

int menu(BITMAP *bufor){

     clear_to_color(bufor, makecol(157,146,001));                                                //wypelniamy bufor kolorem tla
     textprintf_ex( bufor, font, 500, 20, makecol( 200, 200, 200 ), 1, "ROZPOCZNIJ GRE !" );     //
     textprintf_ex( bufor, font, 500, 40, makecol( 200, 200, 200 ), -1, "O GRZE !" );            //i wypisujemy tekst
     textprintf_ex( bufor, font, 500, 60, makecol( 200, 200, 200 ), -1, "WYJSCIE !" );           //
     blit( bufor, screen, 0, 0, 0, 0, 1200, 800 );                                          //wyswielamy gotowy bufor w naszym oknie


    int wskaz=1;                            //zmienna przechowujaca wybor opcji z menu
    bool warunek_menu=true;                 //zmienna logiczna mowiaca o tym czy menu powinno byc jeszcze wyswietlane
    bool bylo_przesuniecie=false;           //zmienna logiczna zapobiegajaca zbyt szybkiej zmianie opcji w menu

while(warunek_menu){

    int ticks=clock();                      //zapietujemy liczbe tikow zegara

    while( (double)(clock()-ticks)/CLOCKS_PER_SEC<0.6){     //wykonuj petle dopoki czas nie przekroczy 0.6s

     if(key[KEY_UP] && !bylo_przesuniecie){                   //jesli strzalak w gore
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

    if(wskaz==1){                                               //w zaleznosci od wskazu wyswietlamy menu z innym zaznaczeniem
     clear_to_color(bufor, makecol(157,146,001));
     textprintf_ex( bufor, font, 500, 20, makecol( 200, 200, 200 ), 1, "ROZPOCZNIJ GRE !" );    //zaznaczone (1 zamiast -1)
     textprintf_ex( bufor, font, 500, 40, makecol( 200, 200, 200 ), -1, "O GRZE !" );
     textprintf_ex( bufor, font, 500, 60, makecol( 200, 200, 200 ), -1, "WYJSCIE !" );
     blit( bufor, screen, 0, 0, 0, 0, 1200, 800 );
    }

     if(wskaz==2){
     clear_to_color(bufor, makecol(157,146,001));
     textprintf_ex( bufor, font, 500, 20, makecol( 200, 200, 200 ), -1, "ROZPOCZNIJ GRE !" );
     textprintf_ex( bufor, font, 500, 40, makecol( 200, 200, 200 ), 1, "O GRZE !" );                //zaznaczone (1 zamiast -1)
     textprintf_ex( bufor, font, 500, 60, makecol( 200, 200, 200 ), -1, "WYJSCIE !" );
     blit( bufor, screen, 0, 0, 0, 0, 1200, 800 );
    }

     if(wskaz==3){
     clear_to_color(bufor, makecol(157,146,001));
     textprintf_ex( bufor, font, 500, 20, makecol( 200, 200, 200 ), -1, "ROZPOCZNIJ GRE !" );
     textprintf_ex( bufor, font, 500, 40, makecol( 200, 200, 200 ), -1, "O GRZE !" );
     textprintf_ex( bufor, font, 500, 60, makecol( 200, 200, 200 ), 1,  "WYJSCIE !" );              //zaznaczone (1 zamiast -1)
     blit( bufor, screen, 0, 0, 0, 0, 1200, 800 );
    }

}
    return wskaz;                   //zwracamy wynik menu

}



//*********************************************************************************************************************
int game(BITMAP *bufor){

    BITMAP * mapa=NULL;                     //wskaünik na bitmape mapy
    BITMAP * waz=NULL;                      //wskaünik na bitmape weza
    BITMAP * pokarm=NULL;                    //wskaünik na bitmape pokarmu

    pokarm = load_bmp( "pokarm.bmp", default_palette );                 //wczytujemy bitmape pokarmu
if( !pokarm ){                                                      //jesli sie nie powiodlo
    set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
    allegro_message( "nie moge zaladowac obrazka pokarm !" );       //wyswietlamy komunikat
    return -1;                                                      //zwracamy kod bledu
}

    mapa = load_bmp( "mapa.bmp", default_palette );                 //wczytujemy bitmape mapy
if( !mapa ){                                                        //jesli sie nie powiodlo
    set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
    allegro_message( "nie moge zaladowac obrazka mapa !" );         //wyswietlamy komunikat
    destroy_bitmap(pokarm);                                       //usuwamy stworzona bitmape z pokarmem
    return -1;                                                      //zwracamy kod bledu
}

    waz = load_bmp( "Waz.bmp", default_palette );               //wczytujemy bitmape weza
if( !waz ){                                                     //jesli sie nie powiodlo
    set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
    allegro_message( "nie moge zaladowac obrazka waz !" );      //wyswietlamy komunikat
    destroy_bitmap(pokarm);                                     //usuwamy stworzona bitmape z pokarmem
    destroy_bitmap(mapa);                                       //i mapa
    return -1;                                                  //zwracamy kod bledu
}

std::vector <wsp> ogony;                    //vector wspolrzednych, w ktorym przechowujemy wsp czesci ciala weza
int snake_x=200, snake_y=200;               //pozycja poczatkowa snake'a
int snake_pos=0;                            //kierunek w ktory jest skierowany snake
bool byl_klawisz=false;                     //flaga zapobiegajaca kikukrotnym wcisniecia klawisza
const double wspolczynnik_predkosci=0.25;
int level=1;                                //aktualny poziom
int score=0;                                //calkowita liczba punktow
int score_tmp=0;                            //dolicznana liczba pkt
int aktualnie_ruchow=0;                     //liczba ruchow wykonanych od zdobycia ostatniego pokarmu
int ruchy_do_pozywienia=0;                  //zmienna przechowujaca minimalna liczbe ruchow potrzebnych do zdobycia pokarmu
int ruchy_x=0;                       //ruchy_do_pozywiania
int ruchy_y=0;                       //rozbite na skladowe x i y

clear_to_color(bufor, makecol(134,78,100));                     //ustawiamy kolor tla

    for(int i=0; i<1000; i+=20){
        for(int j=0; j<800; j+=20){
              masked_blit( mapa, bufor, 0, 0, i, j, mapa->w, mapa->h );         //wypelniamy bufor kafelkami naszego tla
        }
    }
    srand(time(NULL));                                          //
    wsp pozywka;                                               //
    do{                                                         //
        pozywka.x=(rand()%1000)/20;                                 //losujemy wspolrzedne nowej pozywki
        pozywka.x=pozywka.x*20;                                     //
        pozywka.y=(rand()%800)/20;                                  //
        pozywka.y=pozywka.y*20;                                     //
    }while(pozywka.x==snake_x && pozywka.y==snake_y);           //ale nie takie gdzie jest snake

    ruchy_x=pozywka.x-snake_x;                                  //**************************************
    if(ruchy_x<0){                                              //
        ruchy_x=-ruchy_x;                                       //
    }                                                           //
    if(ruchy_x>500){                                            //obliczanie minimalnej liczby ruchow do pokarmu
        ruchy_x=1000-ruchy_x;                                   //
    }                                                           //
                                                                //
    ruchy_y=pozywka.y-snake_y;                                  //
    if(ruchy_y<0){                                              //
        ruchy_y=-ruchy_y;                                       //
    }                                                           //
    if(ruchy_y>500){                                            //
        ruchy_y=1000-ruchy_y;                                   //
    }                                                           //
    ruchy_do_pozywienia=ruchy_x/20 + ruchy_y/20;                //*******************************************

    masked_blit(pokarm,bufor,0,0,pozywka.x,pozywka.y,pokarm->w,pokarm->h);          //wpyelniamy bufor pozywiniem
    masked_blit(waz,bufor,0,(snake_pos*20),snake_x,snake_y,20,20);                  // i wezem
    blit( bufor, screen, 0, 0, 0, 0, 1200, 800 );                                   //wrzucamy bufor na screen

    time_t poczatek=time(NULL);

while( !key[ KEY_ESC ] )                                                            //ESC wychodzi z gry
{
    int ticks=clock();                                                              //tak jak wczesniej tiki zegara do obliczenia czasu
    while( (double)(clock()-ticks)/CLOCKS_PER_SEC<wspolczynnik_predkosci/level){    //czas odswiezania symulacji maleje wraz z levelem co za tym idzie im wyzszy level tym szybciej sie porusza

        if( key[ KEY_LEFT ] ) { if(snake_pos!=0 && byl_klawisz==false) {byl_klawisz=true; snake_pos=1;}}            //
                                                                                                                    //
        if( key[ KEY_RIGHT ] ) { if(snake_pos!=1 && byl_klawisz==false) {byl_klawisz=true; snake_pos=0;}}           //
                                                                                                                    //sprawdzenie wcisnietego przysku i odpowiednia zamiana kierunku snake'a
        if( key[ KEY_UP ] ) { if(snake_pos!=2 && byl_klawisz==false) {byl_klawisz=true; snake_pos=3;}}              //
                                                                                                                    //
        if( key[ KEY_DOWN ] ) {if(snake_pos!=3 && byl_klawisz==false)  {byl_klawisz=true; snake_pos=2;}}            //

    }
    byl_klawisz=false;

    if(snake_pos==0){                   //jesli waz skierowany w prawo
        snake_x+=20;                    //dodajemy do x
        if(snake_x>980)                 //jesli przeszlismy poza plansze
            snake_x=0;                  //pojawiamy sie z drugiej strony
    }
    else if(snake_pos==1){              //jesli waz skierowany w lewo
        snake_x-=20;                    //odejmujemy od x
         if(snake_x<0)
            snake_x=980;
    }
    else if(snake_pos==2){              //jesli waz skierowany w dol
        snake_y+=20;                    //dodajemy do y
         if(snake_y>780)
            snake_y=0;
    }
    else if(snake_pos==3){              //jesli waz skierowany w gore
        snake_y-=20;                    //odejmujemy od y
         if(snake_y<0)
            snake_y=780;
    }

    aktualnie_ruchow++;             //doliczamy aktualny ruch

    if(snake_x==pozywka.x && snake_y==pozywka.y){       //jesli glowa weza jest na polu z pokarmem
        wsp o;                                          //dodajemy nowy segment ciala
        o.x=snake_x;                                //poczotkowo nadajemy wspolrzedne glowy weza
        o.y=snake_y;
        if(snake_pos==0){                             //i odpowiednio modyfikujemy w zaleznosci w jakiej pozycji weza zostal zjedzony pokarm
            o.x-=20;
            if(o.x<0)
                o.x=980;
        }
        else if(snake_pos==1){
            o.x+=20;
            if(o.x>980)
                o.x=0;
        }
        else if(snake_pos==2){
            o.y-=20;
            if(o.y<0)
                o.y=780;
        }
        else if(snake_pos==3){
            o.y+=20;
            if(o.y>780)
                o.y=0;
        }

        ogony.push_back(o);                     //dodajmy nowa czesc do naszej listy


        score_tmp=2*level*100-((aktualnie_ruchow-ruchy_do_pozywienia)*10*level);                //obliczanie punktow za zdobyty pokarm
        if(score_tmp<level*100)
            score_tmp=level*100;

        bool warunek;                                                           //*************************************
        do{                                                                     //losujemy nowe wpolrzedne pokarmu
            warunek=false;                                                      //
            pozywka.x=(rand()%1000)/20;                                         //
            pozywka.x=pozywka.x*20;                                             //
            pozywka.y=(rand()%800)/20;                                          //
            pozywka.y=pozywka.y*20;                                             //
                                                                                //
            if(pozywka.x==snake_x && pozywka.y==snake_y)                        //dopoki nie sa to wsp glowy weza
                warunek=true;                                                   //
                                                                                //
            for(int i=0; i<ogony.size(); i++){                                  //
                if(pozywka.x==ogony[i].x && pozywka.y==ogony[i].y){             //albo wsp jego ciala
                    warunek=true;                                               //
                }                                                               //
            }                                                                   //
        }while(warunek);                                                        //***********************************

                                                                            //*******************
        ruchy_x=pozywka.x-snake_x;                                          //obliczanie minimalnej liczby ruchow do pozywienia
        if(ruchy_x<0){                                                      //
            ruchy_x=-ruchy_x;                                               //
        }                                                                   //
        if(ruchy_x>500){                                                    //
            ruchy_x=1000-ruchy_x;                                           //
        }                                                                   //
        ruchy_y=pozywka.y-snake_y;                                          //
        if(ruchy_y<0){                                                      //
            ruchy_y=-ruchy_y;                                               //
        }                                                                   //
        if(ruchy_y>500){                                                    //
            ruchy_y=1000-ruchy_y;                                           //
        }                                                                   //
        ruchy_do_pozywienia=ruchy_x/20 + ruchy_y/20;                        //*******************
        aktualnie_ruchow=0;                                         //poniewaz wylosowany jest nowy pokarm aktualna liczba ruchow od ostatniego pokarmu jest rowna 0

    }else{                                                          //jesli nie udalo sie zdobyc pokarmu
        if(ogony.size()>=2){                                        //jesli dlugosc weza jest wieksza od 2
            for(int i=0; i<ogony.size()-1; i++){                    //dla kazdej czasci weza
                ogony[i]=ogony[i+1];                                //przesuwamy weza (zapominajac o ostatnim kawalku ktorym zajmiemy sie nizej)
                if(ogony[i].x==snake_x && ogony[i].y==snake_y){       //sprawdzamy czy nie koliduje z glowa
                    clear_to_color(bufor, makecol(55,22,150));                                                                      //jesli wystepuje kolizja wyswietlamy ekran konczacy gre
                    textprintf_ex( bufor, font, 300, 100, makecol( 200, 200, 200 ), - 1, "PRZEGRALES!!!" );
                    textprintf_ex( bufor, font, 300, 130, makecol( 200, 200, 200 ), - 1, "Twoja liczba zdobytych punktow to: %d !", score );
                    textprintf_ex( bufor, font, 300, 160, makecol( 200, 200, 200 ), - 1, "Aby kontynuowac nacisnij ESC" );
                    blit( bufor, screen, 0, 0, 0, 0, 1200, 800 );
                    destroy_bitmap(pokarm);
                    destroy_bitmap(waz);
                    destroy_bitmap(mapa);
                    while(!key[KEY_ESC]);
                    return 0;
                }
            }
        }
        if(!ogony.empty()){                                 //jesli istnieja jakies czesci weza
            ogony[ogony.size()-1].x=snake_x;                //to ostatni kawalek przeskakuje w miejsce glowy w poprzednim ruchu
            ogony[ogony.size()-1].y=snake_y;
            if(snake_pos==0){
                ogony[ogony.size()-1].x-=20;
                if(ogony[ogony.size()-1].x<0)
                    ogony[ogony.size()-1].x=980;
            }
            else if(snake_pos==1){
                ogony[ogony.size()-1].x+=20;
                if(ogony[ogony.size()-1].x>980)
                    ogony[ogony.size()-1].x=0;
            }
            else if(snake_pos==2){
                ogony[ogony.size()-1].y-=20;
                if(ogony[ogony.size()-1].y<0)
                    ogony[ogony.size()-1].y=780;
            }
            else if(snake_pos==3){
                ogony[ogony.size()-1].y+=20;
                if(ogony[ogony.size()-1].y>780)
                    ogony[ogony.size()-1].y=0;
            }
        }

    }

    level=(ogony.size()/5)+1;                       //obliczamy poziom na podstawie dlugosci
    score=score+score_tmp;                          //dodajemy do calkowitego wyniku, wynik tymczasowy
    score_tmp=0;                                    //zerujemy wynik tymczasowy

    clear_to_color(bufor, makecol(134,78,100));

    for(int i=0; i<1000; i+=20){                                            //wypelniamy bufor kafelkami planszy
        for(int j=0; j<800; j+=20){
              masked_blit( mapa, bufor, 0, 0, i, j, mapa->w, mapa->h );
        }
    }

    for(int i=0; i<ogony.size(); i++){                                      //wypelniamy bufor kawalkami weza
       masked_blit(waz,bufor,20,0,ogony[i].x,ogony[i].y,20,20);
    }
    masked_blit(pokarm,bufor,0,0,pozywka.x,pozywka.y,pokarm->w,pokarm->h);      //wypelniamy bufor pozywieniem
    masked_blit(waz,bufor,0,(snake_pos*20),snake_x,snake_y,20,20);              //oraz glowa weza w odpowiedniej pozycji
    textprintf_ex( bufor, font, 1015, 20, makecol( 200, 200, 200 ), - 1, "Poziom: %d !", level );                       //dajemy informacje o poziomie
    textprintf_ex( bufor, font, 1015, 40, makecol( 200, 200, 200 ), - 1, "Liczba punktow: %d !", score );               //liczbie pkt
    textprintf_ex( bufor, font, 1015, 60, makecol( 200, 200, 200 ), - 1, "Czas: %ds !", time(NULL)-poczatek );          //i czasie gry
    blit( bufor, screen, 0, 0, 0, 0, 1200, 800 );                               //wyswietlamy bufor na ekranie okna
}

destroy_bitmap(pokarm);                     //po skonczonej grze sprzatamy po sobie usuwajac wczytane bitmapy
destroy_bitmap(waz);
destroy_bitmap(mapa);
return 0;
}

//************************************************************************************************************************************
int o_grze(BITMAP *bufor){

     clear_to_color(bufor, makecol(87,166,57));
     textprintf_ex( bufor, font, 75, 80, makecol( 200, 200, 200 ), -1, "Klasyczna gra SNAKE. Zbieraj pozywienie uwazajac przy tym aby nie potracic wlasnego ogona." );
     textprintf_ex( bufor, font, 75, 120, makecol( 200, 200, 200 ), -1, "Im dluzszy bedzie waz tym wyzszy poziom, a co za tym idzie wzrasta predkosc rozgrywki." );
     textprintf_ex( bufor, font, 75, 160, makecol( 200, 200, 200 ), -1, "Za kaødy zebrany pokarm dostaniesz okreslona liczbe punktow obliczanπ na podstawie obecnego poziomu oraz tego jak szybko Ci sie to udalo!" );
     textprintf_ex( bufor, font, 75, 200, makecol( 200, 200, 200 ), -1, "Aby poworci nacisnij ESC" );
     blit( bufor, screen, 0, 0, 0, 0, 1200, 800 );

    while(!key[KEY_ESC]);
}

