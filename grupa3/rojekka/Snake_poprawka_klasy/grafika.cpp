#include "grafika.h"

Grafika::Grafika(const char* bitmapa_waz, const char* bitmapa_pokarm, const char* bitmapa_mapa, int szerokosc_okna, int wysokosc_okna){
    _szerokosc_okna = szerokosc_okna;
    _wysokosc_okna = wysokosc_okna;

    allegro_init();                     //zainicjowanie biblioteki allegro
    install_keyboard();                 //aktywowanie klawiatury
    set_color_depth( 16 );              //ustawienie glebi kolorow na 16 bitow
    set_gfx_mode( GFX_AUTODETECT_WINDOWED, szerokosc_okna, wysokosc_okna, 0, 0 );       //utworzenie okna o rozmiarze 1200x800
    clear_to_color( screen, makecol( 128, 128, 128 ) );             //wyczyszczenie tla

                                               //utworzenie wskanika na bitmape naszego bufora
    bufor = create_bitmap( szerokosc_okna, wysokosc_okna );                             //stworzenie bitmapy bufora o wielkosci naszego okna
    if( !bufor ){                                               //jesli sie nie udalo
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "Nie moge utworzyc bufora grafiki !" );
        allegro_exit();                                         //zamykamy allegro
        exit(1);                                              //i konczymy dizlanie aplikacji
    }

    pokarm = load_bmp( bitmapa_pokarm, default_palette );                 //wczytujemy bitmape pokarmu
    if( !pokarm ){                                                      //jesli sie nie powiodlo
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "nie moge zaladowac obrazka pokarm !" );       //wyswietlamy komunikat
        exit(1);                                                       //zwracamy kod bledu
    }

        mapa = load_bmp( bitmapa_mapa, default_palette );                 //wczytujemy bitmape mapy
    if( !mapa ){                                                        //jesli sie nie powiodlo
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "nie moge zaladowac obrazka mapa !" );         //wyswietlamy komunikat
        destroy_bitmap(pokarm);                                       //usuwamy stworzona bitmape z pokarmem
        exit(1);                                                       //zwracamy kod bledu
    }

        waz = load_bmp(bitmapa_waz, default_palette );               //wczytujemy bitmape weza
    if( !waz ){                                                     //jesli sie nie powiodlo
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "nie moge zaladowac obrazka waz !" );      //wyswietlamy komunikat
        destroy_bitmap(pokarm);                                     //usuwamy stworzona bitmape z pokarmem
        destroy_bitmap(mapa);                                       //i mapa
        exit(1);                                                 //zwracamy kod bledu
    }
}

Grafika::~Grafika(){
    destroy_bitmap(bufor);                                  //po skonczeniu glownej petli sprzatamy po sobie i konczymy
    destroy_bitmap(waz);
    destroy_bitmap(mapa);
    destroy_bitmap(pokarm);
    allegro_exit();
}

void Grafika::wypelnijTlo(int R, int G, int B){
    clear_to_color(bufor, makecol(R,G,B));
}

void Grafika::wyswietlBufor(){
    blit( bufor, screen, 0, 0, 0, 0, _szerokosc_okna, _wysokosc_okna );
}

void Grafika::wyswietlTekst( const char* tekst, int x, int y, int inverted, int R, int G, int B){
    textprintf_ex( bufor, font, x, y, makecol( R, G, B ), inverted, tekst);
}

void Grafika::wyswietlPlansze(){
    for(int i=0; i<1000; i+=20){
        for(int j=0; j<800; j+=20){
              masked_blit( mapa, bufor, 0, 0, i, j, mapa->w, mapa->h );         //wypelniamy bufor kafelkami naszego tla
        }
    }
}

void Grafika::wyswietlWeza(Snake& snake){
    masked_blit(waz,bufor,0,(snake.zwrocKierunek()*20),snake.zwrocPozycjeX(),snake.zwrocPozycjeY(),20,20);                  // i wezem
    for(unsigned int i=0; i<snake.zwrocIloscOgonow(); i++){                              //wypelniamy bufor kawalkami weza
       masked_blit(waz,bufor,20,0,snake.ogony[i].x,snake.ogony[i].y,20,20);
    }
}

void Grafika::wyswietlPokarm(Pokarm& jedzenie){
        masked_blit(pokarm,bufor,0,0,jedzenie.zwrocPozycjeX(),jedzenie.zwrocPozycjeY(),pokarm->w,pokarm->h);          //wpyelniamy bufor pozywiniem
}
