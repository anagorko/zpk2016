#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "allegro5/allegro_image.h"

using namespace std;

const float FPS = 60; // Frames Per Second
const int SCREEN_W = 560; // Wysokość ekranu w pikselach
const int SCREEN_H = 640; // Szerokość ekranu w pikselach
const int k_sz = 131; // Szerokość kafelka w pikselach
const int k_wy = 131; // Wysokość kafelka w pikselach
const int liczba_kafelkow = 12; // Liczba plików (obrazów) z kafelkami
const int liczba_opcji = 8; // Liczba plików (obrazów) z opcjami
const int liczba_dzwiekow = 3; // Liczba plików z dzwiękami

enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

ALLEGRO_BITMAP* bitmapa[liczba_kafelkow]; // Tablica przechowująca załadowane obrazy z kafelkami
ALLEGRO_BITMAP* bitmapa_opcje[liczba_opcji]; // Tablica przechowująca załadowane obrazy z opcjami
ALLEGRO_SAMPLE* dzwiek[liczba_dzwiekow]; // Tablica przechowująca załadowane pliki dźwiękowe
ALLEGRO_FONT* font = NULL; // Czcionka do napisu 2048
ALLEGRO_FONT* font_wyniki = NULL; // Czcionka do wyników

const string plik_z_kafelkiem[liczba_kafelkow] =
{
    "media/pusty.png", // pusty
    "media/2.png", // kafelek z numerem 2
    "media/4.png", // kafelek z numerem 4
    "media/8.png", // kafelek z numerem 8
    "media/16.png", // kafelek z numerem 16
    "media/32.png", // kafelek z numerem 32
    "media/64.png", // kafelek z numerem 64
    "media/128.png", // kafelek z numerem 128
    "media/256.png",  // kafelek z numerem 256
    "media/512.png", // kafelek z numerem 512
    "media/1024.png", // kafelek z numerem 1024
    "media/2048.png"  // kafelek z numerem 2048
};

const string plik_z_opcjami[liczba_opcji] =
{
    "media/Nowa_gra_0.png", // Nowa gra
    "media/Nowa_gra_1.png", // Nowa gra
    "media/Cofnij_0.png", // Cofniecie ruchu
    "media/Cofnij_1.png", // Cofniecie ruchu
    "media/Wynik.png", // Wynik
    "media/Najlepszy_wynik.png", //Najlepszy wynik
    "media/Przegrana.png", // Przegrana
    "media/Wygrana.png" // Wygrana
};

const string plik_z_dzwiekiem[liczba_dzwiekow] =
{
    "media/click_x.wav", // Klik
    "media/boo.wav", // Przegrana
    "media/fanfare3.wav" // Wygrana
};

/////////////////////////////  Pomocnicze  /////////////////////////////////////////////

// Funkcja zwraca wartosc prawda, jesli uzytkownik kliknal w wyznaczony obszar
bool Click(int x, int y, int x1, int y1, int x2, int y2){
if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
    return true;
return false;
}

// Wylosowanie liczby (prawdopodobieństwo wylosowania 2 zgodnie z oryginalną grą - 90%)
int losuj_liczbe(){
    double prob = (double)rand() / RAND_MAX;
    int liczba = 0;
    if (prob < 0.9)       //  90%
        liczba = 2;
    else                 //  90% + 10%
        liczba = 4;

    return liczba;
}

// Pole na planszy
struct pole{
    int x;
    int y;
};

// Wylosowanie pola
pole losuj_pole(){
    pole p;
    p.x = rand() % 4;
    p.y = rand() % 4;
    return p;
}

// zmienna przyjmuje wartość 1, jeśli dokonała się zmiana na planszy
bool zmiana = 0;

// Najlepszy wynik we wszystkich grach
int najlepszy_wynik = 0;

////////////////////////////////////////////////////////////////////////////////////////

class Gra {

public:

    int punkty;
    int punkty_poprzednie;
    int plansza[4][4];
    int plansza_wyswietlana[4][4];
    int plansza_stan_poprzedni[4][4];
    int nr_ruchu;

    Gra(): punkty(0), punkty_poprzednie(0), nr_ruchu(0){}

    // uzupe³nienie planszy
    void uzupelnij_plansze();
    // Uaktualnienie planszy zgodnie z przyciskiem wybranym przez u¿ytkownika (zsumowanie pól i przesuniêcie)
    void uaktualnij_plansze(int key);
    // Wyœwietlenie planszy
    void wyswietl_plansze(int plansza[4][4]);
    // Wylosowanie nowej liczby (2 lub 4) i wylosowanie z pustych pól dla niej miejsca
    void nowa_liczba();
    // Cofniêcie ruchu
    void cofnij();
    // Sprawdzenie, czy jest koniec gry (nie ma pustych pól na planszy i u¿ytkownik nie ma jak po³¹czyæ liczb)
    bool czy_koniec();
};

////////////////////////////////////////////////////////////////////////////////////////


// Wyswietlenie planszy startowej
void Gra::uzupelnij_plansze(){
    // Wylosowanie liczb
    int liczba_1 = losuj_liczbe(); int liczba_2 = losuj_liczbe();
    // Wylosowanie pól dla liczb
    pole pole_1 = losuj_pole(); pole pole_2 = losuj_pole();
    // Ponowne losowanie, jesli pola sa takie same
    while(pole_1.x == pole_2.x && pole_1.y == pole_2.y){
        pole_1 = losuj_pole();
    }
    // Uzupełnienie planszy
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            if(i == pole_1.x && j == pole_1.y)
                plansza[i][j] = liczba_1;
            else if(i == pole_2.x && j == pole_2.y)
                plansza[i][j] = liczba_2;
            else
                plansza[i][j] = 0;
        }
    }
};

void Gra::uaktualnij_plansze(int key){

// Zapisanie poprzedniej pozycji planszy i sumy punktów (potrzebne do funkcji cofnij)
    punkty_poprzednie = punkty;
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++){
            plansza_stan_poprzedni[i][j] = plansza[i][j];
        }
    }

// zmienna przyjmuje wartość 1, jeśli dokonała się zmiana na planszy po zsumowaniu pól
bool zmiana_suma = 0;

    if (key == KEY_UP){
        // przesuwanie pola w górę o 1
        for(int i=0;i<4;i++) {
            for(int j=0;j<4;j++){
                if(!plansza[i][j]){
                    for(int k=j+1;k<4;k++)
                        if(plansza[i][k]){
                            plansza[i][j]=plansza[i][k];
                            plansza[i][k]=0;
                            zmiana = 1;
                            break;
                        }
                }
            }
        }
        // sumowanie pól
        for(int i=0;i<4;i++) {
            for(int j=0;j<4;j++){
                if(plansza[i][j] == plansza[i][j+1] && plansza[i][j] != 0){
                    plansza[i][j] += plansza[i][j];
                    plansza[i][j+1] = 0;
                    zmiana_suma = 1;
                    zmiana = 1;
                    punkty += (((log2(plansza[i][j]))-1)*plansza[i][j]);
                    if (plansza[i][j] == 2048) {
                        al_draw_bitmap_region(bitmapa_opcje[7], 0, 0, 562, 333, 0, 100, 0);
                        al_play_sample(dzwiek[3], 1, 0.0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        al_flip_display();
                        al_rest(3);
                    }
                    // Jeśli j jest równe 0, sprawdzenie, czy nie nalęzy zsomować pól dla j = 2 i j = 3
                    if (j == 0)
                        j++;
                    else
                        break;
                }
            }
        }
        // przesuwanie pola w górę o 1 jeszcze raz, jeśli zsumowano jakieś pola
        if(zmiana_suma) {
            for(int i=0;i<4;i++) {
                for(int j=0;j<4;j++){
                    if(!plansza[i][j]){
                        for(int k=j+1;k<4;k++)
                            if(plansza[i][k]){
                                plansza[i][j]=plansza[i][k];
                                plansza[i][k]=0;
                                zmiana_suma = 0;
                                break;
                            }
                    }
                }
            }
        }
    }
    if (key == KEY_DOWN){
        // przesuwanie pola w dół o 1
        for(int i=0;i<4;i++) {
            for(int j=3;j>=0;j--){
                if(!plansza[i][j]){
                    for(int k=j-1;k>=0;k--)
                        if(plansza[i][k]){
                            plansza[i][j]=plansza[i][k];
                            plansza[i][k]=0;
                            zmiana = 1;
                            break;
                        }
                }
            }
        }
        // sumowanie pól
        for(int i=0;i<4;i++) {
            for(int j=3;j>=0;j--){
                if(plansza[i][j] == plansza[i][j-1] && plansza[i][j] != 0){
                    plansza[i][j] += plansza[i][j];
                    plansza[i][j-1] = 0;
                    zmiana = 1;
                    zmiana_suma = 1;
                    punkty += (((log2(plansza[i][j]))-1)*plansza[i][j]);
                    if (plansza[i][j] == 2048) {
                        al_draw_bitmap_region(bitmapa_opcje[7], 0, 0, 562, 333, 0, 100, 0);
                        al_flip_display();
                        al_play_sample(dzwiek[3], 1, 0.0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        al_rest(3);
                    }
                    // Jeśli j jest równe 3, sprawdzenie, czy nie nalęzy zsomować pól dla j = 1 i j = 0
                    if (j == 3)
                        j--;
                    else
                        break;
                }
            }
        }
        // przesuwanie pola w dół o 1 jeszcze raz, jeśli zsumowano jakieś pola
        if(zmiana_suma){
            for(int i=0;i<4;i++) {
                for(int j=3;j>=0;j--){
                    if(!plansza[i][j]){
                        for(int k=j-1;k>=0;k--)
                            if(plansza[i][k]){
                                plansza[i][j]=plansza[i][k];
                                plansza[i][k]=0;
                                zmiana = 1;
                                zmiana_suma = 0;
                                break;
                            }
                    }
                }
            }
        }
    }
    if (key == KEY_LEFT){
        // przesuwanie pola w lewo o 1
        for(int j=0;j<4;j++) {
            for(int i=0;i<4;i++){
                if(!plansza[i][j]){
                    for(int k=i+1;k<4;k++)
                        if(plansza[k][j]){
                            plansza[i][j]=plansza[k][j];
                            plansza[k][j]=0;
                            zmiana = 1;
                            break;
                        }
                }
            }
        }
        // sumowanie pól
        for(int j=0;j<4;j++) {
            for(int i=0;i<4;i++){
                if(plansza[i][j] == plansza[i+1][j]&& plansza[i][j] != 0){
                    plansza[i][j] += plansza[i][j];
                    plansza[i+1][j] = 0;
                    zmiana = 1;
                    zmiana_suma = 1;
                    punkty += (((log2(plansza[i][j]))-1)*plansza[i][j]);
                    if (plansza[i][j] == 2048) {
                        al_draw_bitmap_region(bitmapa_opcje[7], 0, 0, 562, 333, 0, 100, 0);
                        al_flip_display();
                        al_play_sample(dzwiek[3], 1, 0.0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        al_rest(3);
                    }
                    // Jeśli i jest równe 0, sprawdzenie, czy nie nalęzy zsomować pól dla i = 2 i i = 3
                    if (i == 0)
                        i++;
                    else
                        break;
                }
            }
        }
        // przesuwanie pola w lewo o 1 jeszcze raz, jeśli zsumowano jakieś pola
        if(zmiana_suma) {
            for(int j=0;j<4;j++) {
                for(int i=0;i<4;i++){
                    if(!plansza[i][j]){
                        for(int k=i+1;k<4;k++)
                            if(plansza[k][j]){
                                plansza[i][j]=plansza[k][j];
                                plansza[k][j]=0;
                                zmiana = 1;
                                zmiana_suma = 0;
                                break;
                            }
                    }
                }
            }
        }
    }
    if (key == KEY_RIGHT){
        // przesuwanie pola w prawo o 1
        for(int j=0;j<4;j++) {
            for(int i=3;i>=0;i--){
                if(!plansza[i][j]){
                    for(int k=i-1;k>=0;k--)
                        if(plansza[k][j]){
                            plansza[i][j]=plansza[k][j];
                            plansza[k][j]=0;
                            zmiana = 1;
                            break;
                        }
                }
            }
        }
        // sumowanie pól
        for(int j=0;j<4;j++) {
            for(int i=3;i>=0;i--){
                if(plansza[i][j] == plansza[i-1][j] && plansza[i][j] != 0){
                    plansza[i][j] += plansza[i][j];
                    plansza[i-1][j] = 0;
                    zmiana = 1;
                    zmiana_suma = 1;
                    punkty += (((log2(plansza[i][j]))-1)*plansza[i][j]);
                    if (plansza[i][j] == 2048) {
                        al_draw_bitmap_region(bitmapa_opcje[7], 0, 0, 562, 333, 0, 100, 0);
                        al_flip_display();
                        al_play_sample(dzwiek[3], 1, 0.0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        al_rest(3);
                    }
                    // Jeśli i jest równe 3, sprawdzenie, czy nie nalęzy zsomować pól dla i = 0 i i = 1
                    if (i == 3)
                        i--;
                    else
                        break;
                }
            }
        }
        // przesuwanie pola w prawo o 1 jeszcze raz, jeśli zsumowano jakieś pola
        if (zmiana_suma) {
            for(int j=0;j<4;j++) {
                for(int i=3;i>=0;i--){
                    if(!plansza[i][j]){
                        for(int k=i-1;k>=0;k--)
                            if(plansza[k][j]){
                                plansza[i][j]=plansza[k][j];
                                plansza[k][j]=0;
                                zmiana = 1;
                                zmiana_suma = 0;
                                break;
                            }
                    }
                }
            }
        }
    }
}

// Wylosowanie nowej liczby (2 lub 4) i wybranie dla niej miejsca
void Gra::nowa_liczba(){
    // Wylosowanie liczby
    int liczba = losuj_liczbe();
    // Wylosowanie pola dla liczby
    pole nowe_pole = losuj_pole();
    while (plansza[nowe_pole.x][nowe_pole.y] != 0){
       nowe_pole = losuj_pole();
    }
    // Wstawienie pola na planszę
    plansza[nowe_pole.x][nowe_pole.y] = liczba;
    zmiana = 0;
};

// Cofnięcie ruchu
void Gra::cofnij(){

    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++){
            plansza[i][j] = plansza_stan_poprzedni[i][j];
        }
    }
}

// Sprawdzenie, czy jest koniec gry (nie ma pustych pól na planszy i użytkownik nie ma jak połączyć liczb)
bool Gra::czy_koniec(){

   // Sprawdzenie, czy wszystkie pola są zapełnione
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++){
            if (plansza[i][j] == 0)
                return false;
        }
    }
    // Sprawdzenie, czy można wykonać kolejny ruch
        // Sprawdzenie, czy występuje takie samo pole w pionie
         for(int i=0;i<4;i++) {
            for(int j=0;j<3;j++){
                if(plansza[i][j] == plansza[i][j+1])
                    return false;
            }
         }
        // Sprawdzenie, czy występuje takie samo pole w poziomie
         for(int j=0;j<4;j++) {
            for(int i=0;i<3;i++){
                if(plansza[i][j] == plansza[i][j+1])
                    return false;
            }
         }
    return true;
}

// Wyswietlenie planszy
void Gra::wyswietl_plansze(int plansza[4][4]){

    al_clear_to_color(al_map_rgb(100,100,100));

    // Dopasowanie wartości na planszy wyświelanej użytkownikowi zgodnej z kolejnymi numerkami tablicy stringów z kafelkami
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            switch(plansza[i][j]) {
            case 0:
               plansza_wyswietlana[i][j] = 0;
               break;
            case 2:
               plansza_wyswietlana[i][j] = 1;
               break;
            case 4:
               plansza_wyswietlana[i][j] = 2;
               break;
            case 8:
               plansza_wyswietlana[i][j] = 3;
               break;
            case 16:
               plansza_wyswietlana[i][j] = 4;
               break;
            case 32:
               plansza_wyswietlana[i][j] = 5;
               break;
            case 64:
               plansza_wyswietlana[i][j] = 6;
               break;
            case 128:
               plansza_wyswietlana[i][j] = 7;
               break;
            case 256:
               plansza_wyswietlana[i][j] = 8;
               break;
            case 512:
               plansza_wyswietlana[i][j] = 9;
               break;
            case 1024:
               plansza_wyswietlana[i][j] = 10;
               break;
            case 2048:
               plansza_wyswietlana[i][j] = 11;
               break;
            }
        }
    }

    // Wyświetlenie planszy gry
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            al_draw_bitmap_region(bitmapa[plansza_wyswietlana[i][j]], 0, 0, k_sz, k_wy, i*k_sz+20, j*k_sz+100, 0);
        }
    }

    //wstawienie nazwy gry "2048"
    al_draw_text(font, al_map_rgb(220,220,220), 25, 30, 0, "2048");

    // Wyświetlenie opcji (nowa gra, cofnij)
    al_draw_bitmap_region(bitmapa_opcje[0], 0, 0, 115, 32, 155, 20, 0);
    al_draw_bitmap_region(bitmapa_opcje[2], 0, 0, 115, 32, 155, 55, 0);

    // Wyświetlenie bitmap z wynikiem i najlepszym wynikiem
    al_draw_bitmap_region(bitmapa_opcje[4], 0, 0, 133, 69, 280, 20, 0);
    al_draw_bitmap_region(bitmapa_opcje[5], 0, 0, 133, 69, 413, 20, 0);

    al_flip_display();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{

   //////////////////////////// INICJALIZACJA ////////////////////////////////

   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   bool key[4] = {false, false, false, false};
   bool redraw = true;
   bool doexit = false;

    // ALLEGRO
    if(!al_init()) {
        fprintf(stderr, "Blad podczas inicjalizacji biblioteki Allegro\n");
        return -1;
    }

    // KLAWIATURA I MYSZKA
    if(!al_install_keyboard()) {
        fprintf(stderr, "Blad podczas inicjalizacji klawiatury!\n");
        return -1;
    }
    if(!al_install_mouse()) {
      fprintf(stderr, "Blad podczas inicjalizacji myszki!\n");
      return -1;
    }
    ALLEGRO_MOUSE_STATE state;
    int x,y;

    // DŹWIĘK
    if(!al_install_audio()){
      fprintf(stderr, "Blad poczas inicjalizacji audio!\n");
      return -1;
    }
    if(!al_init_acodec_addon()){
      fprintf(stderr, "Blad podczas inicjalizacji audio codeków!\n");
      return -1;
    }
    if (!al_reserve_samples(1)){ // jeden dźwięk w tle
      fprintf(stderr, "Nie udalo sie zarezerwować miejsca na plik dzwiekowy!\n");
      return -1;
    }
    for (int i = 0; i < liczba_dzwiekow; i++) {
        dzwiek[i] = al_load_sample(plik_z_dzwiekiem[i].c_str());
        if(!dzwiek[i]) {
            cerr << "Blad podczas wczytywania pliku z dzwiekiem " << plik_z_dzwiekiem[i] << "." << endl;
            return -1;
        }
    }

    // TIMER
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        fprintf(stderr, "Nie udalo sie zainicjalizowac timera!\n");
        return -1;
    }

    // EKRAN
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
        fprintf(stderr, "Nie udalo sie stworzyc ekranu!\n");
        al_destroy_timer(timer);
        return -1;
    }
    al_set_window_title(display,"ZPK 2016/2017, GRA 2048");

    // OBRAZ
    if(!al_init_image_addon()) {
        fprintf(stderr, "Nie udalo sie zainicjalizowac al_init_image_addon!\n");
        al_destroy_timer(timer);
        return -1;
    }
    for (int i = 0; i < liczba_kafelkow; i++) {
        bitmapa[i] = al_load_bitmap(plik_z_kafelkiem[i].c_str());
        if(!bitmapa[i]) {
            cerr << "Blad podczas wczytywania bitmapy " << plik_z_kafelkiem[i] << "." << endl;
            al_destroy_display(display);
            al_destroy_timer(timer);
            return -1;
        }
    }
    for (int i = 0; i < liczba_opcji; i++) {
        bitmapa_opcje[i] = al_load_bitmap(plik_z_opcjami[i].c_str());
        if(!bitmapa_opcje[i]) {
            cerr << "Blad podczas wczytywania bitmapy " << plik_z_opcjami[i] << "." << endl;
            al_destroy_display(display);
            al_destroy_timer(timer);
            return -1;
        }
    }

    // KURSOR
    ALLEGRO_BITMAP* kursor  = al_load_bitmap("media/kursor.png");
    if (!kursor){
      fprintf(stderr, "Nie udalo się wczytać kursora.\n");
      return -1;
    }
    ALLEGRO_MOUSE_CURSOR* cursor = al_create_mouse_cursor(kursor, 0, 0);

    // CZCIONKA
    al_init_font_addon(); // inicjalizacja czcionki
    al_init_ttf_addon();// inicjalizacja ttf (True Type Font)
    font = al_load_ttf_font("media/orange juice 2.0.ttf", 60, 0);
    if (!font){
      fprintf(stderr, "Nie udalo sie zainicjowac czcionki 'orange juice 2.0.ttf'.\n");
      return -1;
    }
    font_wyniki = al_load_ttf_font("media/Franklin Gothic Medium Regular.ttf", 40, 0);
    if (!font_wyniki){
      fprintf(stderr, "Nie udalo sie zainicjowac czcionki 'Franklin Gothic Medium Regular.ttf'.\n");
      return -1;
    }

    // KOLEJKA ZDARZEŃ
    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "Nie udalo sie stworzyc kolejki zdarzen!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_mouse_event_source());
   al_register_event_source(event_queue, al_get_keyboard_event_source());

   al_clear_to_color(al_map_rgb(0,0,0));
   al_flip_display();
   al_start_timer(timer);

//////////////// WŁAŚCIWA CZĘŚĆ GRY ////////////////////

    Gra gra;

    // Ustawienie pseudolosowości liczb
    srand(time(NULL));

NowaGra:

    gra.uzupelnij_plansze();
    gra.wyswietl_plansze(gra.plansza);

//
// Event loop - główna pętla programu
//
   while(!doexit)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
    ///////////// KLAWIATURA /////////////
      if(ev.type == ALLEGRO_EVENT_TIMER) {
         if(key[KEY_UP]) {
            gra.uaktualnij_plansze(KEY_UP);
            if (zmiana){
                gra.nr_ruchu++;
                gra.nowa_liczba();
                zmiana = 0;
                if (gra.czy_koniec() == true) {
                    al_draw_bitmap_region(bitmapa_opcje[6], 0, 0, 562, 333, 0, 100, 0);
                    al_flip_display();
                    al_play_sample(dzwiek[1], 0.3, 0.0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                    al_rest(3);
                    if (gra.punkty > najlepszy_wynik) {
                        najlepszy_wynik = gra.punkty;
                    }
                    gra.punkty = 0;
                    key[KEY_UP] = false;
                    goto NowaGra;
                }
            }
            key[KEY_UP] = false;
         }
         if(key[KEY_DOWN]) {
            gra.uaktualnij_plansze(KEY_DOWN);
            if (zmiana){
                gra.nr_ruchu++;
                gra.nowa_liczba();
                zmiana = 0;
                if (gra.czy_koniec() == true) {
                    al_draw_bitmap_region(bitmapa_opcje[6], 0, 0, 562, 333, 0, 100, 0);
                    al_flip_display();
                    al_play_sample(dzwiek[1], 0.3, 0.0, 1, ALLEGRO_PLAYMODE_ONCE,NULL);
                    al_rest(3);
                    if (gra.punkty > najlepszy_wynik) {
                        najlepszy_wynik = gra.punkty;
                    }
                    gra.punkty = 0;
                    key[KEY_DOWN] = false;
                    goto NowaGra;
                }
            }
            key[KEY_DOWN] = false;
         }
         if(key[KEY_LEFT]) {
            gra.uaktualnij_plansze(KEY_LEFT);
            if (zmiana){
                gra.nr_ruchu++;
                gra.nowa_liczba();
                zmiana = 0;
                if (gra.czy_koniec() == true) {
                    al_draw_bitmap_region(bitmapa_opcje[6], 0, 0, 562, 333, 0, 100, 0);
                    al_flip_display();
                    al_play_sample(dzwiek[1], 0.3, 0.0, 1, ALLEGRO_PLAYMODE_ONCE,NULL);
                    al_rest(3);
                    if (gra.punkty > najlepszy_wynik) {
                        najlepszy_wynik = gra.punkty;
                    }
                    gra.punkty = 0;
                    key[KEY_LEFT] = false;
                    goto NowaGra;
                }
            }
            key[KEY_LEFT] = false;
         }
         if(key[KEY_RIGHT]) {
            gra.uaktualnij_plansze(KEY_RIGHT);
            if (zmiana){
                gra.nr_ruchu++;
                gra.nowa_liczba();
                zmiana = 0;
                if (gra.czy_koniec() == true) {
                    al_draw_bitmap_region(bitmapa_opcje[6], 0, 0, 562, 333, 0, 100, 0);
                    al_flip_display();
                    al_play_sample(dzwiek[1], 0.3, 0.0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                    al_rest(3);
                    if (gra.punkty > najlepszy_wynik) {
                        najlepszy_wynik = gra.punkty;
                    }
                    gra.punkty = 0;
                    key[KEY_RIGHT] = false;
                    goto NowaGra;
                }
            }
                key[KEY_RIGHT] = false;
            }
            redraw = true;
        }
        ///////////// PRZYCISK ZAMKNIJ ///////////////
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
        }
        ///////////// MYSZ ///////////////
        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            al_get_mouse_state(&state); // pobierane sa wspolrzedne klikniecia
            x = al_get_mouse_state_axis(&state,0);
            y = al_get_mouse_state_axis(&state,1);

            if(Click(x,y,155,20,270,50)){ //NOWA GRA - symulacja wcisniecia przycisku
                al_play_sample(dzwiek[0], 1.0, 0.0,2.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                al_draw_bitmap_region(bitmapa_opcje[1], 0, 0, 115, 32, 155, 20, 0);
                al_flip_display();
                al_rest(0.15);
                al_draw_bitmap_region(bitmapa_opcje[0], 0, 0, 115, 32, 155, 20, 0);
                al_flip_display();
                gra.punkty = 0;
                goto NowaGra;
            }

            if(Click(x,y,155,55,270,85)){ //COFNIJ - symulacja wcisniecia przycisku
                al_play_sample(dzwiek[0], 1.0, 0.0,2.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                al_draw_bitmap_region(bitmapa_opcje[3], 0, 0, 115, 32, 155, 55, 0);
                al_flip_display();
                al_rest(0.15);
                al_draw_bitmap_region(bitmapa_opcje[2], 0, 0, 115, 32, 155, 55, 0);
                al_flip_display();
                // Nie można cofnąć ruchu, gdy się jeszcze nie ruszyło
                if (gra.nr_ruchu > 0) {
                    gra.punkty = gra.punkty_poprzednie;
                    gra.cofnij();
                }
            }

        }
      ///////////// INICJALIZACJA PRZYCISKU NA KLAWIATURZE ///////////////
      else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = true;
               break;

            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = true;
               break;

            case ALLEGRO_KEY_LEFT:
               key[KEY_LEFT] = true;
               break;

            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = true;
               break;
         }
      }
      ///////////// WYŚWIETLENIE OBRAZU ///////////////
      if(redraw && al_is_event_queue_empty(event_queue)) {
        redraw = false;
        al_clear_to_color(al_map_rgb(100,100,100));
        gra.wyswietl_plansze(gra.plansza);
        al_draw_textf(font_wyniki, al_map_rgb(255,255,255), 345, 38, ALLEGRO_ALIGN_CENTRE, "%d", gra.punkty);
        al_draw_textf(font_wyniki, al_map_rgb(255,255,255), 475, 38, ALLEGRO_ALIGN_CENTRE, "%d", najlepszy_wynik);
        al_set_mouse_cursor(display, cursor );
        al_flip_display();
      }

   }
    ///////////// ZWOLNIENIE PAMIĘCI ///////////////
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);

    return 0;
}
