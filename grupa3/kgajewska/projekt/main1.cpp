//szablon do kodu: https://wiki.allegro.cc/index.php?title=Basic_Keyboard_Example
//https://wiki.allegro.cc/index.php?title=Allegro_5_Tutorial%2FAddons%2FFonts

#include <iostream>
#include <string>
#include<vector>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

using namespace std;


//zbiór wykorzystywanych w grze klawiszy
enum MYKEYS {
   KEY_LEFT, KEY_RIGHT, KEY_SPACE
};
bool key[3] = { false, false, false }; //tablica sluzaca do indetyfikacji aktualnie wcisnietych klawiszy

enum EKRAN {
    EKRAN_STARTOWY, EKRAN_GRY, EKRAN_KONCOWY
};

struct Obiekt {
    float x;
    float y;
    int szerokosc;
    int wysokosc;
};


class Plansza {

    enum ZWROT {
        PRAWO,
        LEWO
    };

    // zmienne dotyczace gry
    float FPS;
    int SCREEN_W;
    int SCREEN_H;
    int LICZBA_KOLUMN;
    float ODLEGLOSC; // marginesy dla wrogow
    int wynik;
    int poziom;

    // zmienne dotyczace gracza
    int GRACZ_SIZE_X;
    int GRACZ_SIZE_Y;
    Obiekt gracz;
    int zycia;
    int czas_do_strzalu;
    int czas_wybuch; // czas wyswietlania wybuchu

    // zmienne dotyczace wrogow
    int WROG_SIZE_X;
    int WROG_SIZE_Y;
    vector<Obiekt> wrogowie;
    ZWROT obecny_zwrot;
    int wychylenie;

    // zmienne dotyczace pociskow
    int POCISK_SIZE_X;
    int POCISK_SIZE_Y;
    vector<Obiekt> pociski_wrogow;
    vector<Obiekt> pociski_gracza;



public:
    bool kolizja(Obiekt o1, Obiekt o2);
    Plansza(float fps, int ekran_szer, int ekran_wys, int gracz_szer, int gracz_wys, int poc_szer, int poc_wys, int wrog_szer, int wrog_wys);
    void ustaw_poziom(int poziom);
    void nastepny_poziom();
    bool sprawdz_zwyciestwo();
    bool sprawdz_porazke();
    bool pokaz_wybuch();
    void przesun_wrogow();
    void przesun_pociski(float dy_gracz, float dy_wrog);
    void przesun_gracza(float dx);
    void strzel();
    void losuj_strzaly_wroga();
    void usun_stare_pociski();
    void sprawdz_czy_zestrzelono_wroga();
    void sprawdz_czy_zestrzelono_gracza();
    void kasuj_czas_do_strzalu();
    void kasuj_wynik();
    void ustaw_zycie(int z);
    void zmniejsz_czas_wybuchu();
    vector<Obiekt> daj_wrogow();
    Obiekt daj_gracza();
    vector<Obiekt> daj_pociski_gracza();
    vector<Obiekt> daj_pociski_wroga();
    int daj_szerokosc_ekranu();
    int daj_wysokosc_ekranu();
    int daj_szerokosc_pocisku();
    int daj_wysokosc_pocisku();
    int daj_szerokosc_gracza();
    int daj_wysokosc_gracza();
    int daj_szerokosc_wroga();
    int daj_wysokosc_wroga();
    int daj_wynik();
    int daj_zycia();
    int daj_poziom();
    float daj_fps();

};

bool Plansza::kolizja(Obiekt o1, Obiekt o2) {
    if (o1.y < o2.y && o2.y < o1.y + o1.wysokosc){
        if ((o1.x < o2.x && o2.x < o1.x + o1.szerokosc) || (o1.x < o2.x + o2.szerokosc && o2.x + o2.szerokosc <  o1.x + o1.szerokosc)){
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

Plansza::Plansza(float fps, int ekran_szer, int ekran_wys, int gracz_szer, int gracz_wys, int poc_szer, int poc_wys, int wrog_szer, int wrog_wys) {
    FPS = fps;
    SCREEN_W = ekran_szer;
    SCREEN_H = ekran_wys;
    GRACZ_SIZE_X = gracz_szer;
    GRACZ_SIZE_Y = gracz_wys;
    gracz.szerokosc = gracz_szer;
    gracz.wysokosc = gracz_wys;
    POCISK_SIZE_X = poc_szer;
    POCISK_SIZE_Y = poc_wys;
    WROG_SIZE_X = wrog_szer;
    WROG_SIZE_Y = wrog_wys;
    LICZBA_KOLUMN = 10;
    ODLEGLOSC = 50.0;
    wynik = 0;
    poziom = 1;
    zycia = 3;
    czas_do_strzalu = 0;
    ustaw_poziom(poziom);
}

void Plansza::ustaw_poziom(int poziom) {
    this->poziom = poziom;
    czas_wybuch = 0;

    // ustawiam pozycje gracza
    gracz.x = SCREEN_W / 2.0 - gracz.szerokosc / 2.0;
    gracz.y = SCREEN_H - gracz.wysokosc;

    // zeruje liste pociskow
    pociski_gracza.clear();
    pociski_wrogow.clear();

    // aktualizacja liczby wrogow dla nowej planszy
    wychylenie = 0;
    obecny_zwrot = PRAWO;

    float obecne_x = ODLEGLOSC;
    float obecne_y = ODLEGLOSC;
    wrogowie.clear();
    int liczba_wrogow = 10 * poziom;
    float przerwa = (SCREEN_W - 2 * ODLEGLOSC - LICZBA_KOLUMN * WROG_SIZE_X) / (LICZBA_KOLUMN - 1);
    for (int i = 0; i < liczba_wrogow; i++){
        Obiekt wrog;
        wrog.x = obecne_x;
        wrog.y = obecne_y;
        wrog.szerokosc = WROG_SIZE_X;
        wrog.wysokosc = WROG_SIZE_Y;
        wrogowie.push_back(wrog);
        if (i % LICZBA_KOLUMN == 9) {
            obecne_x = ODLEGLOSC;
            obecne_y += (WROG_SIZE_Y + przerwa);
        }
        else {
            obecne_x += (WROG_SIZE_X + przerwa);
        }
    }
}

void Plansza::nastepny_poziom() {
    poziom++;
    ustaw_poziom(poziom);
}

bool Plansza::sprawdz_zwyciestwo() {
    return wrogowie.size() == 0;
}

bool Plansza::sprawdz_porazke() {
    if (zycia <= 0) {
        return true;
    }
    for (int i = 0; i < wrogowie.size(); i++) {
        if (wrogowie[i].y > SCREEN_H - wrogowie[i].wysokosc) {
            return true;
        }
    }
    return false;
}

bool Plansza::pokaz_wybuch() {
    return czas_wybuch > 0;
}

void Plansza::przesun_wrogow() {
    for (int i = 0; i < wrogowie.size(); i++){
        if (obecny_zwrot == PRAWO){
            wrogowie[i].x += 1.0;
        }
        else{
            wrogowie[i].x -= 1;
        }
    }

    if (obecny_zwrot == PRAWO) {
        wychylenie++;
    }
    else {
        wychylenie--;
    }

    //spr koniecznosci zmiany zwrotu ruchu wroga
    if (wychylenie == 40){
        obecny_zwrot = LEWO;
        for (int i = 0; i < wrogowie.size(); i++) {
            wrogowie[i].y += 3.0;
        }
    }
    else if (wychylenie == -40){
        obecny_zwrot = PRAWO;
        for (int i = 0; i < wrogowie.size(); i++) {
            wrogowie[i].y += 3.0;
        }
    }
}

void Plansza::przesun_pociski(float dy_gracz, float dy_wrog) {
    for (int i = 0; i < pociski_gracza.size(); i++){
        pociski_gracza[i].y -= dy_gracz;
    }
    for (int i = 0; i < pociski_wrogow.size(); i++){
        pociski_wrogow[i].y += dy_wrog;
    }
}

void Plansza::przesun_gracza(float dx) {
    if (dx < 0 && gracz.x >= -dx) {
        gracz.x += dx;
    }
    if (dx > 0 && gracz.x <= SCREEN_W - GRACZ_SIZE_X - dx) {
        gracz.x += dx;
    }
}

// strzal gracza
void Plansza::strzel() {
    if (czas_do_strzalu == 0) {
        Obiekt pocisk;
        pocisk.x = gracz.x + GRACZ_SIZE_X/2 - POCISK_SIZE_X/2;
        pocisk.y = gracz.y;
        pocisk.szerokosc = POCISK_SIZE_X;
        pocisk.wysokosc = POCISK_SIZE_Y;
        pociski_gracza.push_back(pocisk);
        czas_do_strzalu = FPS;
    }
    else {
        czas_do_strzalu--;
    }
}

// tworzy losowo kilka strzalow wroga
void Plansza::losuj_strzaly_wroga() {
    if (rand() % 60 == 0 && wrogowie.size() > 0){
        int nr_strzelca = rand() % wrogowie.size();
        Obiekt pocisk;
        pocisk.x = wrogowie[nr_strzelca].x + wrogowie[nr_strzelca].szerokosc/2 - POCISK_SIZE_X/2;
        pocisk.y = wrogowie[nr_strzelca].y + wrogowie[nr_strzelca].wysokosc;
        pocisk.szerokosc = POCISK_SIZE_X;
        pocisk.wysokosc = POCISK_SIZE_Y;
        pociski_wrogow.push_back(pocisk);
    }
}

// usuwa pociski, ktore znalazly sie poza plansza
void Plansza::usun_stare_pociski() {
    for (int i = 0; i < pociski_gracza.size(); i++){
        if (pociski_gracza[i].y < 0){
            pociski_gracza.erase(pociski_gracza.begin()+i);
        }
    }
    for (int i = 0; i < pociski_wrogow.size(); i++){
        if (pociski_wrogow[i].y > SCREEN_H){
            pociski_wrogow.erase(pociski_wrogow.begin()+i);
        }
    }
}

// sprawdza, czy zestrzelono ktoregos wroga. Jesli tak, to usuwa i jego, i pocisk oraz zwieksza wynik
void Plansza::sprawdz_czy_zestrzelono_wroga() {
    for (int i = 0; i < wrogowie.size() ; i++){
        for (int j = 0; j < pociski_gracza.size(); j++){
            if (kolizja(wrogowie[i], pociski_gracza[j])){
                wynik += 10;
                wrogowie.erase(wrogowie.begin()+i);
                pociski_gracza.erase(pociski_gracza.begin()+j);
                break;
            }
        }
    }
}

// sprawdza, czy zestrzelono gracza. jesli tak, to wyswietla wybuch i zmniejsza zycie
void Plansza::sprawdz_czy_zestrzelono_gracza() {
    for (int i = 0; i < pociski_wrogow.size(); i++){
        if (kolizja(gracz, pociski_wrogow[i])){
            pociski_wrogow.erase(pociski_wrogow.begin()+i);
            zycia--;
            czas_wybuch = 10;
        }
    }
}

void Plansza::kasuj_czas_do_strzalu() {
    czas_do_strzalu = 0;
}

void Plansza::kasuj_wynik() {
    wynik = 0;
}

void Plansza::ustaw_zycie(int z) {
    zycia = z;
}

void Plansza::zmniejsz_czas_wybuchu() {
    if (czas_wybuch > 0) {
        czas_wybuch--;
    }
}

Obiekt Plansza::daj_gracza() {
    return gracz;
}

vector<Obiekt> Plansza::daj_wrogow() {
    return wrogowie;
}

vector<Obiekt> Plansza::daj_pociski_gracza() {
    return pociski_gracza;
}

vector<Obiekt> Plansza::daj_pociski_wroga() {
    return pociski_wrogow;
}

int Plansza::daj_zycia() {
    return zycia;
}

int Plansza::daj_wynik() {
    return wynik;
}

int Plansza::daj_szerokosc_ekranu() {
    return SCREEN_W;
}

int Plansza::daj_wysokosc_ekranu() {
    return SCREEN_H;
}

int Plansza::daj_szerokosc_pocisku() {
    return POCISK_SIZE_X;
}
int Plansza::daj_wysokosc_pocisku() {
    return POCISK_SIZE_Y;
}
int Plansza::daj_szerokosc_gracza() {
    return GRACZ_SIZE_X;
}
int Plansza::daj_wysokosc_gracza() {
    return GRACZ_SIZE_Y;
}
int Plansza::daj_szerokosc_wroga() {
    return WROG_SIZE_X;
}
int Plansza::daj_wysokosc_wroga() {
    return WROG_SIZE_Y;
}

int Plansza::daj_poziom() {
    return poziom;
}

float Plansza::daj_fps() {
    return FPS;
}

int main(int argc, char **argv)
{
    srand( time(NULL) );
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_BITMAP *tlo = NULL;
    ALLEGRO_BITMAP *gracz_bitmapa = NULL;
    ALLEGRO_BITMAP *pocisk_bitmapa = NULL;
    ALLEGRO_BITMAP *wrog_bitmapa = NULL;
    ALLEGRO_BITMAP *wybuch_bitmapa = NULL;
    ALLEGRO_BITMAP *koniec_gry_porazka = NULL;
    ALLEGRO_BITMAP *koniec_gry_zwyciestwo = NULL;

    bool redraw = true;
    bool doexit = false;
    bool zwyciestwo = false;
    float czas_gry = 0.0;
    EKRAN obecny_ekran = EKRAN_STARTOWY;

    Plansza plansza = Plansza(60.0, 640, 480, 60,20, 4, 10, 32, 32);

    // inicjalizacja obieków
    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }

    al_init_font_addon();
    al_init_ttf_addon();
    if (!al_init_image_addon()) {
        return -1;
    }

    timer = al_create_timer(1.0 / plansza.daj_fps());
    display = al_create_display(plansza.daj_szerokosc_ekranu(), plansza.daj_wysokosc_ekranu());
    font = al_load_ttf_font("media/font.ttf", 20, 0);
    tlo = al_load_bitmap("media/tlo.png");
    gracz_bitmapa = al_load_bitmap("media/gracz.png");
    pocisk_bitmapa = al_create_bitmap(plansza.daj_szerokosc_pocisku(), plansza.daj_wysokosc_pocisku());
    al_set_target_bitmap(pocisk_bitmapa);
    al_clear_to_color(al_map_rgb(255, 0, 0));
    al_set_target_bitmap(al_get_backbuffer(display));
    wrog_bitmapa = al_load_bitmap("media/wrog.png");
    wybuch_bitmapa = al_load_bitmap("media/wybuch.png");
    koniec_gry_porazka = al_load_bitmap("media/gameover.png");
    koniec_gry_zwyciestwo = al_load_bitmap("media/win.png");
    if(!timer || !display || !font || !tlo || !gracz_bitmapa || !pocisk_bitmapa || !wrog_bitmapa || !wybuch_bitmapa || !koniec_gry_porazka || !koniec_gry_zwyciestwo) {
        fprintf(stderr, "failed to create bitmap\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_font(font);
        al_destroy_bitmap(tlo);
        al_destroy_bitmap(gracz_bitmapa);
        al_destroy_bitmap(pocisk_bitmapa);
        al_destroy_bitmap(wrog_bitmapa);
        al_destroy_bitmap(wybuch_bitmapa);
        al_destroy_bitmap(koniec_gry_porazka);
        al_destroy_bitmap(koniec_gry_zwyciestwo);
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_font(font);
        al_destroy_bitmap(tlo);
        al_destroy_bitmap(gracz_bitmapa);
        al_destroy_bitmap(pocisk_bitmapa);
        al_destroy_bitmap(wrog_bitmapa);
        al_destroy_bitmap(wybuch_bitmapa);
        al_destroy_bitmap(koniec_gry_porazka);
        al_destroy_bitmap(koniec_gry_zwyciestwo);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_clear_to_color(al_map_rgb(0,0,0));

    al_flip_display();
    al_start_timer(timer);

    // glowna petla gry
    while(!doexit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (obecny_ekran == EKRAN_STARTOWY) {
            al_draw_bitmap(tlo, 0, 0, 0);
            char tekst[50];
            sprintf(tekst, "Naciśnij ENTER, aby rozpocząć.");
            al_draw_text(font, al_map_rgb(255,255,255), plansza.daj_szerokosc_ekranu()/2, plansza.daj_wysokosc_ekranu()/2 , ALLEGRO_ALIGN_CENTER, tekst);
            al_flip_display();
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
            if(ev.type == ALLEGRO_EVENT_KEY_UP) {
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ENTER:
                        obecny_ekran = EKRAN_GRY;
                        plansza.ustaw_poziom(1);
                        plansza.kasuj_wynik();
                        czas_gry = 0.0;
                        break;
                }
            }
        }
        else if (obecny_ekran == EKRAN_KONCOWY) {
            al_draw_bitmap(tlo, 0, 0, 0);
            if (zwyciestwo) {
                al_draw_bitmap(koniec_gry_zwyciestwo, plansza.daj_szerokosc_ekranu()/2-128.0, plansza.daj_wysokosc_ekranu()/6, 0);
                char tekst[100];
                sprintf(tekst, "Pokonałeś wroga w %d sekund.", int(czas_gry));
                al_draw_text(font, al_map_rgb(255,255,255), plansza.daj_szerokosc_ekranu()/2, plansza.daj_wysokosc_ekranu()/2, ALLEGRO_ALIGN_CENTER, tekst);
            }
            else {
                al_draw_bitmap(koniec_gry_porazka, plansza.daj_szerokosc_ekranu()/2-128.0, plansza.daj_wysokosc_ekranu()/6, 0);
            }
            al_draw_text(font, al_map_rgb(255,255,255), plansza.daj_szerokosc_ekranu()/2, plansza.daj_wysokosc_ekranu()*3/4 , ALLEGRO_ALIGN_CENTER, "Naciśnij ENTER, aby zacząć od nowa.");
            al_flip_display();
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
            if(ev.type == ALLEGRO_EVENT_KEY_UP) {
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ENTER:
                        obecny_ekran = EKRAN_GRY;
                        plansza.ustaw_poziom(1);
                        plansza.kasuj_wynik();
                        plansza.ustaw_zycie(3);
                        czas_gry = 0.0;
                        break;
                }
            }
        }
        else if (obecny_ekran == EKRAN_GRY) {
            if(ev.type == ALLEGRO_EVENT_TIMER) {
                // obsluga zdarzen zwiazanych z cyklicznym odswiezaniem ekranu
                czas_gry += 1.0/plansza.daj_fps();
                if (plansza.sprawdz_zwyciestwo()) {
                    plansza.nastepny_poziom();
                }
                if (plansza.daj_poziom() == 8) {
                    zwyciestwo = true;
                    obecny_ekran = EKRAN_KONCOWY;
                    continue;
                }

                if (plansza.sprawdz_porazke()) {
                    zwyciestwo = false;
                    obecny_ekran = EKRAN_KONCOWY;
                }

                if (key[KEY_LEFT]) {
                    plansza.przesun_gracza(-4.0);
                }


                if(key[KEY_RIGHT]) {
                    plansza.przesun_gracza(4.0);
                }

                if(key[KEY_SPACE]) {
                    plansza.strzel();
                }

                plansza.przesun_wrogow();
                plansza.przesun_pociski(12.0, 5.0);
                plansza.sprawdz_czy_zestrzelono_gracza();
                plansza.sprawdz_czy_zestrzelono_wroga();
                plansza.usun_stare_pociski();
                plansza.losuj_strzaly_wroga();
                plansza.zmniejsz_czas_wybuchu();

                redraw = true;
            }
            else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_LEFT:
                        key[KEY_LEFT] = true;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        key[KEY_RIGHT] = true;
                        break;
                    case ALLEGRO_KEY_SPACE:
                        key[KEY_SPACE] = true;
                        plansza.kasuj_czas_do_strzalu();
                        break;
                }
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_LEFT:
                        key[KEY_LEFT] = false;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        key[KEY_RIGHT] = false;
                        break;
                    case ALLEGRO_KEY_SPACE:
                        key[KEY_SPACE] = false;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        doexit = true;
                        break;
                }
            }
            // odswiezenie ekranu
            if(redraw && al_is_event_queue_empty(event_queue)) {
                redraw = false;
                al_draw_bitmap(tlo, 0, 0, 0);
                char tekst[50];
                sprintf(tekst, "Punkty: %d. Życia: %d. Poziom: %d. Czas: %d", plansza.daj_wynik(), plansza.daj_zycia(), plansza.daj_poziom(), (int)czas_gry);
                al_draw_text(font, al_map_rgb(255,255,255), 5, 5 , ALLEGRO_ALIGN_LEFT, tekst);

                vector<Obiekt> pociski_gracza = plansza.daj_pociski_gracza();
                vector<Obiekt> pociski_wroga = plansza.daj_pociski_wroga();
                vector<Obiekt> wrogowie = plansza.daj_wrogow();
                Obiekt gracz = plansza.daj_gracza();
                for (int i = 0; i < pociski_gracza.size(); i++){
                    al_draw_bitmap(pocisk_bitmapa, pociski_gracza[i].x, pociski_gracza[i].y, 0);
                }
                for (int i = 0; i < pociski_wroga.size(); i++){
                    al_draw_bitmap(pocisk_bitmapa, pociski_wroga[i].x, pociski_wroga[i].y, 0);
                }
                for (int i = 0; i < wrogowie.size(); i++){
                    al_draw_bitmap(wrog_bitmapa, wrogowie[i].x, wrogowie[i].y, 0);
                }
                al_draw_bitmap(gracz_bitmapa, gracz.x, gracz.y, 0);
                if (plansza.pokaz_wybuch()) {
                    al_draw_bitmap(wybuch_bitmapa, gracz.x-20.0, gracz.y-40.0, 0);
                }
                al_flip_display();
            }
        }
    }

    al_destroy_bitmap(gracz_bitmapa);
    al_destroy_bitmap(pocisk_bitmapa);
    al_destroy_bitmap(wrog_bitmapa);
    al_destroy_bitmap(tlo);
    al_destroy_bitmap(wybuch_bitmapa);
    al_destroy_bitmap(koniec_gry_porazka);
    al_destroy_bitmap(koniec_gry_zwyciestwo);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
