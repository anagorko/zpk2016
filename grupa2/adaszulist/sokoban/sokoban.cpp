#include <iostream>
#include <fstream>
#include <allegro5/allegro_primitives.h>
#include<allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>



using namespace std;
const int SCREEN_W = 800;//obrazek 40x40
const int SCREEN_H = 680;
//const int SOKOBAN_SIZE = 32;


int main(){

    ALLEGRO_DISPLAY *display;

    if(!al_init())
        al_show_native_message_box(NULL, "Error", NULL, "Could not Initialize Allegro", NULL, NULL);

    display = al_create_display(SCREEN_W, SCREEN_H);

    if(!display)
        al_show_native_message_box(NULL, "Error", NULL, "Could not create Allegro Display", NULL, NULL);

    al_set_window_position(display, 300, 50);//!pozycja gdzie okienko sie wyswietla, znajdz cos co ustawi okno na srodku

    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();//zeby png chodzilo
    al_init_font_addon();
    al_init_ttf_addon();

    al_clear_to_color(al_map_rgb(255,255,255));




    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    ALLEGRO_BITMAP *kot = al_load_bitmap("kot.png")  ;
    ALLEGRO_BITMAP *sciana = al_load_bitmap("sciana.png");
    ALLEGRO_BITMAP *skrzynka = al_load_bitmap("skrzynka.png");
    ALLEGRO_BITMAP *tlo = al_load_bitmap("tlo.png");
    ALLEGRO_BITMAP *SnM = al_load_bitmap("SnM.png");
    ALLEGRO_BITMAP *KnM = al_load_bitmap("KnM.png");
    ALLEGRO_BITMAP *miejsce = al_load_bitmap("miejsce.png");
  ALLEGRO_FONT *font = al_load_ttf_font("arial.ttf",18,NULL );



    ifstream plik;
    string listaPlikow[]={"level0.txt","level1.txt","level2.txt", "level3.txt", "level20.txt"};
    int numerPliku=0;
    bool startGry=true;
    int mapa[17][20];
    int polozenie[2];
    int poprzedni; // poprzednie miejsce stania ludzika, przechowuje czy stal na "3"
    int ilosc; //ile skrzynek na mapie
    int ok; //wszystkie skrzynki na miejscach (ile skrzynek na miejscach)
    bool startPoziomu; //czy nadal ten sam poziom czy ma przejsc do nastepnego
    bool naMiejscu; // ludek na miejscu na skrzynke "3"

    while(startGry==true){
    plik.open( listaPlikow[numerPliku] );
    naMiejscu = false;
    startPoziomu=true;
    ok=0;

    for(int w=0; w<17; w++){//czy nie skonczyly sie wiersze
        for(int k=0; k<20; k++){//kolumny
            plik >> mapa[w][k];
            if (mapa[w][k]==4) {
                polozenie[0]=w;
                polozenie[1]=k;
            }
        }
    }
    plik>> ilosc;
    plik.close();

    while(startPoziomu==true){   //pierwsza petla, sprawdza czy ludzik stal na miejscu na skrzynke
        for(int w=0; w<17; w++){//czy nie skonczyly sie wiersze
            for(int k=0; k<20; k++){
                if(mapa[w][k]==1){
                al_draw_bitmap(sciana, k*40, w*40, 0);
                }
                else if(mapa[w][k]==2){
                al_draw_bitmap(skrzynka, k*40, w*40, 0);
                }
                else if(mapa[w][k]==5){
                al_draw_bitmap(SnM, k*40, w*40, 0);
                }
                else if(mapa[w][k]==3){
                al_draw_bitmap(miejsce, k*40, w*40, 0);
                }
                else if(mapa[w][k]==4 && naMiejscu==false){//kot
                al_draw_bitmap(kot, k*40, w*40, 0);
                }
                else if(mapa[w][k]==4 && naMiejscu==true){ //kot na miejscu na skrzynki
                al_draw_bitmap(KnM, k*40, w*40, 0);
                }
                else if(mapa[w][k]==0){
                al_draw_bitmap(tlo, k*40, w*40, 0);
                }
            }
         }

         al_draw_textf(font, al_map_rgb(255,174,201), 10, 10,0, "SCORE: %d/%d", ok, ilosc);
         al_draw_textf(font, al_map_rgb(255,174,201), 710, 10,0, "LEVEL: %d", numerPliku);
        al_flip_display();
        //al_rest(0.1);



        if(naMiejscu==false){
            poprzedni=0; //nie stal na 3
        }
        else if(naMiejscu==true){
            poprzedni=3; //stal na 3 wczesniej
        }
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        if(events.type == ALLEGRO_EVENT_KEY_DOWN)//WCISNIETY JAKIS KLAWISZ (JAKIKOLWIEK)
        {
            switch(events.keyboard.keycode)
            {
            case ALLEGRO_KEY_LEFT:  /*!!!!!!!!!!!!!!!!!!!!RUCH W LEWO!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
                if (mapa[polozenie[0]][polozenie[1]-1]==0){  //!!obok wolne
                    polozenie[1]=polozenie[1]-1; //zmiana polozenia
                    mapa[polozenie[0]][polozenie[1]+1]=poprzedni;
                    mapa[polozenie[0]][polozenie[1]]=4;
                    naMiejscu=false;
                }

                else if (mapa[polozenie[0]][polozenie[1]-1]==2){ //!!skrzynka obok
                    if(mapa[polozenie[0]][polozenie[1]-2]==3){ //!3-2-4
                        polozenie[1]=polozenie[1]-1;
                        mapa[polozenie[0]][polozenie[1]+1]=poprzedni; //czy stal wczesniej na 3 czy nie
                        mapa[polozenie[0]][polozenie[1]-1]=5;//5, bo skrzynka stanela na 3
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=false; // nie stoi na miejscu na skrzynke
                        ok++;
                    }
                    else if (mapa[polozenie[0]][polozenie[1]-2]==0){ //!0-2-4, sasiadem skrzynki jest wolne
                        polozenie[1]=polozenie[1]-1;
                        mapa[polozenie[0]][polozenie[1]+1]=poprzedni;
                        mapa[polozenie[0]][polozenie[1]-1]=2;
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=false; //tutaj ludzik nie stoi na 3 bo mial obok 2
                    }
                }
                else if (mapa[polozenie[0]][polozenie[1]-1]==3){  //!!obok ludzika "3"
                    polozenie[1]=polozenie[1]-1; //zmiana polozenia
                    mapa[polozenie[0]][polozenie[1]+1]=poprzedni;
                    mapa[polozenie[0]][polozenie[1]]=4;
                    naMiejscu=true;
                }
                else if(mapa[polozenie[0]][polozenie[1]-1]==5){ //!!obok ludka skrzynka na polu na skrzynki
                    if(mapa[polozenie[0]][polozenie[1]-2]==3){ //!3-5-4
                        polozenie[1]=polozenie[1]-1;
                        mapa[polozenie[0]][polozenie[1]+1]=poprzedni;
                        mapa[polozenie[0]][polozenie[1]-1]=5;//5
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=true;
                    }
                    else if (mapa[polozenie[0]][polozenie[1]-2]==0){ //!2-5-4
                        polozenie[1]=polozenie[1]-1;
                        mapa[polozenie[0]][polozenie[1]+1]=poprzedni;
                        mapa[polozenie[0]][polozenie[1]-1]=2;
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=true;
                        ok--;
                    }
                }
                break;

            case ALLEGRO_KEY_RIGHT: /*!!!!!!!!!!!!!!!!!!!!!!!!!RUCH W PRAWO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
              if (mapa[polozenie[0]][polozenie[1]+1]==0){  //!obok wolne
                    polozenie[1]=polozenie[1]+1; //zmiana polozenia
                    mapa[polozenie[0]][polozenie[1]-1]=poprzedni;
                    mapa[polozenie[0]][polozenie[1]]=4;
                    naMiejscu=false;
                }
                else if (mapa[polozenie[0]][polozenie[1]+1]==2){ //!skrzynka obok
                    if(mapa[polozenie[0]][polozenie[1]+2]==3){ //!3-2-4
                        polozenie[1]=polozenie[1]+1;
                        mapa[polozenie[0]][polozenie[1]-1]=poprzedni; //czy stal wczesniej na 3 czy nie
                        mapa[polozenie[0]][polozenie[1]+1]=5;//5, bo skrzynka stanela na 3
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=false; // nie stoi na miejscu na skrzynke
                        ok++;

                    }
                    else if (mapa[polozenie[0]][polozenie[1]+2]==0){ //!0-2-4, sasiadem skrzynki jest wolne
                        polozenie[1]=polozenie[1]+1;
                        mapa[polozenie[0]][polozenie[1]-1]=poprzedni;
                        mapa[polozenie[0]][polozenie[1]+1]=2;
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=false; //tutaj ludzik nie stoi na 3 bo mial obok 2
                    }
                }
                else if (mapa[polozenie[0]][polozenie[1]+1]==3){  //!obok ludzika "3"
                    polozenie[1]=polozenie[1]+1; //zmiana polozenia
                    mapa[polozenie[0]][polozenie[1]-1]=poprzedni;
                    mapa[polozenie[0]][polozenie[1]]=4;
                    naMiejscu=true;
                }
                else if(mapa[polozenie[0]][polozenie[1]+1]==5){ //!obok ludka skrzynka na polu na skrzynki
                    if(mapa[polozenie[0]][polozenie[1]+2]==3){ //!3-5-4
                        polozenie[1]=polozenie[1]+1;
                        mapa[polozenie[0]][polozenie[1]-1]=poprzedni;
                        mapa[polozenie[0]][polozenie[1]+1]=5;//5
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=true;
                    }
                    else if (mapa[polozenie[0]][polozenie[1]+2]==0){//!0-5-4
                        polozenie[1]=polozenie[1]+1;
                        mapa[polozenie[0]][polozenie[1]-1]=poprzedni;
                        mapa[polozenie[0]][polozenie[1]+1]=2;
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=true;
                        ok--;
                    }
                }
                break;

        case ALLEGRO_KEY_UP: /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!RUCH W GORE!!!!!!!!!!!!!!!!!!!!!!!!!*/
            if (mapa[polozenie[0]-1][polozenie[1]]==0){  //!obok wolne
                    polozenie[0]=polozenie[0]-1; //zmiana polozenia
                    mapa[polozenie[0]+1][polozenie[1]]=poprzedni;
                    mapa[polozenie[0]][polozenie[1]]=4;
                    naMiejscu=false;
                }
                else if (mapa[polozenie[0]-1][polozenie[1]]==2){ //!skrzynka obok//
                    if(mapa[polozenie[0]-2][polozenie[1]]==3){ //! 3-2-4
                        polozenie[0]=polozenie[0]-1;
                        mapa[polozenie[0]+1][polozenie[1]]=poprzedni; //czy stal wczesniej na 3 czy nie
                        mapa[polozenie[0]-1][polozenie[1]]=5;//5, bo skrzynka stanela na 3
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=false;
                        ok++;// nie stoi na miejscu na skrzynke
                    }
                    else if (mapa[polozenie[0]-2][polozenie[1]]==0){ //!0-2-4, sasiadem skrzynki jest wolne
                        polozenie[0]=polozenie[0]-1;
                        mapa[polozenie[0]+1][polozenie[1]]=poprzedni;
                        mapa[polozenie[0]-1][polozenie[1]]=2;
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=false; //tutaj ludzik nie stoi na 3 bo mial obok 2
                    }
                }
                else if (mapa[polozenie[0]-1][polozenie[1]]==3){  //!!obok ludzika "3"
                    polozenie[0]=polozenie[0]-1; //zmiana polozenia
                    mapa[polozenie[0]+1][polozenie[1]]=poprzedni;
                    mapa[polozenie[0]][polozenie[1]]=4;
                    naMiejscu=true;
                }
                else if(mapa[polozenie[0]-1][polozenie[1]]==5){ //!!obok ludka skrzynka na polu na skrzynki
                    if(mapa[polozenie[0]-2][polozenie[1]]==3){ //!3-5-4
                        polozenie[0]=polozenie[0]-1;
                        mapa[polozenie[0]+1][polozenie[1]]=poprzedni;
                        mapa[polozenie[0]-1][polozenie[1]]=5;//5
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=true;
                    }
                    else if (mapa[polozenie[0]-2][polozenie[1]]==0){ //!0-5-4
                        polozenie[0]=polozenie[0]-1;
                        mapa[polozenie[0]+1][polozenie[1]]=poprzedni;
                        mapa[polozenie[0]-1][polozenie[1]]=2;
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=true;
                        ok--;
                    }
                }
                break;

        case ALLEGRO_KEY_DOWN: /*!!!!!!!!!!!!!!!!!!!!!!!!RUCH W DOL!!!!!!!!!!!!!!!!!!!!!!!!*/
            if (mapa[polozenie[0]+1][polozenie[1]]==0){  //!obok wolne
                    polozenie[0]=polozenie[0]+1; //zmiana polozenia
                    mapa[polozenie[0]-1][polozenie[1]]=poprzedni;
                    mapa[polozenie[0]][polozenie[1]]=4;
                    naMiejscu=false;
                }
                else if (mapa[polozenie[0]+1][polozenie[1]]==2){ //!skrzynka obok//
                    if(mapa[polozenie[0]+2][polozenie[1]]==3){ //! 3-2-4
                        polozenie[0]=polozenie[0]+1;
                        mapa[polozenie[0]-1][polozenie[1]]=poprzedni; //czy stal wczesniej na 3 czy nie
                        mapa[polozenie[0]+1][polozenie[1]]=5;//5, bo skrzynka stanela na 3
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=false;
                        ok++;// nie stoi na miejscu na skrzynke
                    }
                    else if (mapa[polozenie[0]+2][polozenie[1]]==0){ //!0-2-4, sasiadem skrzynki jest wolne
                        polozenie[0]=polozenie[0]+1;
                        mapa[polozenie[0]-1][polozenie[1]]=poprzedni;
                        mapa[polozenie[0]+1][polozenie[1]]=2;
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=false; //tutaj ludzik nie stoi na 3 bo mial obok 2
                    }
                }
                else if (mapa[polozenie[0]+1][polozenie[1]]==3){  //!!obok ludzika "3"
                    polozenie[0]=polozenie[0]+1; //zmiana polozenia
                    mapa[polozenie[0]-1][polozenie[1]]=poprzedni;
                    mapa[polozenie[0]][polozenie[1]]=4;
                    naMiejscu=true;
                }
                else if(mapa[polozenie[0]+1][polozenie[1]]==5){ //!!obok ludka skrzynka na polu na skrzynki
                    if(mapa[polozenie[0]+2][polozenie[1]]==3){ //!3-5-4
                        polozenie[0]=polozenie[0]+1;
                        mapa[polozenie[0]-1][polozenie[1]]=poprzedni;
                        mapa[polozenie[0]+1][polozenie[1]]=5;
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=true;
                    }
                    else if (mapa[polozenie[0]+2][polozenie[1]]==0){ //!0-5-4
                        polozenie[0]=polozenie[0]+1;
                        mapa[polozenie[0]-1][polozenie[1]]=poprzedni;
                        mapa[polozenie[0]+1][polozenie[1]]=2;
                        mapa[polozenie[0]][polozenie[1]]=4;
                        naMiejscu=true;
                        ok--;
                    }
                }
                break;
            case ALLEGRO_KEY_ESCAPE:
            startPoziomu=false;
            cout<< "esc";
            break;
            }//zamyka case
        }// zamyka wcisniecie jakiegokolwiek klawisza


    if(ok==ilosc){
        startPoziomu=false;

        numerPliku++;

        if(numerPliku==5){
            startGry=false;
        }

    }//ILOSC
    }//while start poziomu
    }//while start gry

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}//main

