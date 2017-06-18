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

struct pocisk {
    float x;
    float y;
};

struct wrog {
    float x;
    float y;
};

//ustawienia ekranu
const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
// ustawienia gracza
const int GRACZ_SIZE_X = 60;
const int GRACZ_SIZE_Y = 30;
float gracz_x;
float gracz_y;
int zycie = 3;
int wynik = 0;
int poziom = 1;
int czas_do_strzalu = 0;
bool zwyciestwo = false;
int pokaz_wybuch = 0; // liczba klatek wyswietlajacych wybuch
float czas_gry = 0.0;
//ustawienia dla pocisku
const int POCISK_SIZE_X = 4;
const int POCISK_SIZE_Y = 10;
std::vector <struct pocisk> pociski;
std::vector <struct pocisk> pociski_wroga;
//ustawienia dla wrogów
const int WROG_SIZE_X = 32;
const int WROG_SIZE_Y = 32;
int liczba_wrogow = 5;
const int LICZBA_RZEDOW = 3;
const int LICZBA_KOLUMN = 10;
const float ODLEGLOSC = 50.0; // ODLEGLOSĆ OD BRZEGU
std::vector <struct wrog> wrogowie;

enum ZWROT {
    PRAWO, LEWO
};
ZWROT obecny_zwrot = PRAWO;
int wychylenie = 0; // wrogowie są na środku ekranu

//zbiór wykorzystywanych w grze klawiszy
enum MYKEYS {
   KEY_LEFT, KEY_RIGHT, KEY_SPACE
};
bool key[3] = { false, false, false }; //tablica sluzaca do indetyfikacji aktualnie wcisnietych klawiszy

enum EKRAN {
    EKRAN_STARTOWY, EKRAN_GRY, EKRAN_KONCOWY
};
EKRAN obecny_ekran = EKRAN_STARTOWY;

// definicje kolizji

bool kolizja_gracz (float g_x, float g_y, struct pocisk p){
    if (g_y < p.y + POCISK_SIZE_Y && p.y + GRACZ_SIZE_Y < g_y + GRACZ_SIZE_Y){
        if ((g_x < p.x && p.x < g_x + GRACZ_SIZE_X) || (g_x < p.x + POCISK_SIZE_X && p.x + POCISK_SIZE_X <  g_x + GRACZ_SIZE_X)){
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

bool kolizja_wrog (struct wrog w , struct pocisk p){
    if (w.y < p.y && p.y < w.y + WROG_SIZE_Y){
        if ((w.x < p.x && p.x < w.x + WROG_SIZE_X) || (w.x < p.x + POCISK_SIZE_X && p.x + POCISK_SIZE_X <  w. x + WROG_SIZE_X)){
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

void tworz_plansze(int poziom) {
    pokaz_wybuch = 0;
    zwyciestwo = false;
    // zeruje tablice wcisnietych klawiszy
    for (int i = 0; i < 3; i++) {
        key[i] = false;
    }
    // ustawiam pozycje gracza
    gracz_x = SCREEN_W / 2.0 - GRACZ_SIZE_X / 2.0;
    //float gracz_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
    gracz_y = SCREEN_H - GRACZ_SIZE_Y;

    // zeruje liste pociskow
    pociski.clear();
    pociski_wroga.clear();

    // aktualizacja liczby wrogow dla nowej planszy
    wychylenie = 0;
    float obecne_x = ODLEGLOSC;
    float obecne_y = ODLEGLOSC;
    wrogowie.clear();
    liczba_wrogow = 10 * poziom;
    float przerwa = (SCREEN_W - 2 * ODLEGLOSC - LICZBA_KOLUMN * WROG_SIZE_X) / (LICZBA_KOLUMN - 1);
    for (int i = 0; i < liczba_wrogow; i++){
        struct wrog w;
        w.x = obecne_x;
        w.y = obecne_y;
        wrogowie.push_back(w);
        if (i % LICZBA_KOLUMN == 9) {
            obecne_x = ODLEGLOSC;
            obecne_y += (WROG_SIZE_Y + przerwa);
        }
        else {
            obecne_x += (WROG_SIZE_X + przerwa);
        }
    }
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

    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        fprintf(stderr, "failed to create timer!\n");
        al_destroy_timer(timer);
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        return -1;
    }

    font = al_load_ttf_font("media/font.ttf", 20, 0);
    if(!font) {
        fprintf(stderr, "failed to create font!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_font(font);
        return -1;
    }

    tlo = al_load_bitmap("media/tlo.png");
    if(!tlo ) {
        fprintf(stderr, "failed to create bitmap TŁO!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_font(font);
        al_destroy_bitmap(tlo);
        return -1;
    }

    gracz_bitmapa = al_load_bitmap("media/gracz.png");
    if(!gracz_bitmapa) {
        fprintf(stderr, "failed to create gracz bitmap GRACZ!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_font(font);
        al_destroy_bitmap(tlo);
        al_destroy_bitmap(gracz_bitmapa);
        return -1;
    }

    pocisk_bitmapa = al_create_bitmap(POCISK_SIZE_X, POCISK_SIZE_Y);
    if(!pocisk_bitmapa) {
        fprintf(stderr, "failed to create gracz bitmap POCISK!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_font(font);
        al_destroy_bitmap(tlo);
        al_destroy_bitmap(gracz_bitmapa);
        al_destroy_bitmap(pocisk_bitmapa);
        return -1;
    }
    al_set_target_bitmap(pocisk_bitmapa);
    al_clear_to_color(al_map_rgb(255, 0, 0));

    wrog_bitmapa = al_load_bitmap("media/wrog.png");
    if(!wrog_bitmapa) {
        fprintf(stderr, "failed to create gracz bitmap WROG!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_font(font);
        al_destroy_bitmap(tlo);
        al_destroy_bitmap(gracz_bitmapa);
        al_destroy_bitmap(pocisk_bitmapa);
        al_destroy_bitmap(wrog_bitmapa);
        return -1;
    }

    wybuch_bitmapa = al_load_bitmap("media/wybuch.png");
    if(!wybuch_bitmapa) {
        fprintf(stderr, "failed to create gracz bitmap WYBUCH!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_font(font);
        al_destroy_bitmap(tlo);
        al_destroy_bitmap(gracz_bitmapa);
        al_destroy_bitmap(pocisk_bitmapa);
        al_destroy_bitmap(wrog_bitmapa);
        al_destroy_bitmap(wybuch_bitmapa);
        return -1;
    }

    koniec_gry_porazka = al_load_bitmap("media/gameover.png");
    if(!koniec_gry_porazka) {
        fprintf(stderr, "failed to create gracz bitmap KONIECGRY!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_font(font);
        al_destroy_bitmap(tlo);
        al_destroy_bitmap(gracz_bitmapa);
        al_destroy_bitmap(pocisk_bitmapa);
        al_destroy_bitmap(wrog_bitmapa);
        al_destroy_bitmap(wybuch_bitmapa);
        al_destroy_bitmap(koniec_gry_porazka);
        return -1;
    }

    koniec_gry_zwyciestwo = al_load_bitmap("media/win.png");
    if(!koniec_gry_zwyciestwo) {
        fprintf(stderr, "failed to create gracz bitmap KONIECGRY!\n");
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

    al_set_target_bitmap(al_get_backbuffer(display));

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
            al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2 , ALLEGRO_ALIGN_CENTER, tekst);
            al_flip_display();
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
            if(ev.type == ALLEGRO_EVENT_KEY_UP) {
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ENTER:
                        obecny_ekran = EKRAN_GRY;
                        poziom = 1;
                        wynik = 0;
                        czas_gry = 0.0;
                        tworz_plansze(poziom);
                        break;
                }
            }
        }
        else if (obecny_ekran == EKRAN_KONCOWY) {
            al_draw_bitmap(tlo, 0, 0, 0);
            if (zwyciestwo) {
                al_draw_bitmap(koniec_gry_zwyciestwo, SCREEN_W/2-128.0, SCREEN_H/6, 0);
                char tekst[100];
                sprintf(tekst, "Pokonałeś wroga w %d sekund.", int(czas_gry));
                al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTER, tekst);
            }
            else {
                al_draw_bitmap(koniec_gry_porazka, SCREEN_W/2-128.0, SCREEN_H/6, 0);
            }
            al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H*3/4 , ALLEGRO_ALIGN_CENTER, "Naciśnij ENTER, aby zacząć od nowa.");
            al_flip_display();
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
            if(ev.type == ALLEGRO_EVENT_KEY_UP) {
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ENTER:
                        zycie = 3;
                        obecny_ekran = EKRAN_GRY;
                        poziom = 1;
                        wynik = 0;
                        czas_gry = 0.0;
                        tworz_plansze(poziom);
                        break;
                }
            }
        }
        else if (obecny_ekran == EKRAN_GRY) {
            if(ev.type == ALLEGRO_EVENT_TIMER) {
            // obsluga zdarzen zwiazanych z cyklicznym odswiezaniem ekranu
                czas_gry += 1.0/FPS;
                if(wrogowie.size() == 0) {
                    poziom++;
                    tworz_plansze(poziom);
                }
                // sprawdzam, czy jakis wrog dotarl do konca ekranu
                for (int i = 0; i < wrogowie.size(); i++) {
                    if (wrogowie[i].y > SCREEN_H - WROG_SIZE_Y) {
                        obecny_ekran = EKRAN_KONCOWY;
                        zwyciestwo = false;
                    }
                }
                if(zycie <= 0) {
                    obecny_ekran = EKRAN_KONCOWY;
                }
                if(poziom == 8) {
                    obecny_ekran = EKRAN_KONCOWY;
                    zwyciestwo = true;
                    continue;
                }

                if(key[KEY_LEFT] && gracz_x >= 4.0) {
                    gracz_x -= 4.0;
                }

                if(key[KEY_RIGHT] && gracz_x <= SCREEN_W - GRACZ_SIZE_X - 4.0) {
                    gracz_x += 4.0;
                }

                if(key[KEY_SPACE]) {
                    if (czas_do_strzalu == 0) {
                        struct pocisk p;
                        p.x= gracz_x + GRACZ_SIZE_X/2 - POCISK_SIZE_X/2;
                        p.y = gracz_y;
                        pociski.push_back(p);
                        czas_do_strzalu = FPS;
                    }
                    else {
                        czas_do_strzalu--;
                    }
                }

                for (int i = 0; i < pociski.size(); i++){
                    if (pociski[i].y < 40){
                        pociski.erase(pociski.begin()+i);
                    }
                }

                for (int i = 0; i < pociski_wroga.size(); i++){
                    if (pociski_wroga[i].y > SCREEN_H){
                        pociski_wroga.erase(pociski_wroga.begin()+i);
                    }
                }

                //aktualizuje pozycje pocisków
                for (int i = 0; i < pociski.size(); i++){
                    pociski[i].y -= 12.0;
                }
                for (int i = 0; i < pociski_wroga.size(); i++){
                    pociski_wroga[i].y += 5.0; // typ!!!
                }

                // sprawdzamy kolizje pocisk <-> wgrog
                for (int i = 0; i < wrogowie.size() ; i++){
                    for (int j = 0; j < pociski.size(); j++){
                        if (kolizja_wrog(wrogowie[i], pociski[j])){
                            wynik += 10;
                            wrogowie.erase(wrogowie.begin()+i);
                            pociski.erase(pociski.begin()+j);
                            break;
                        }
                    }
                }

                for (int i = 0; i < pociski_wroga.size(); i++){
                    if (kolizja_gracz(gracz_x, gracz_y, pociski_wroga[i])){
                        pociski_wroga.erase(pociski_wroga.begin()+i);
                        zycie--;
                        pokaz_wybuch = 10;
                    }
                }

                // ruch wroga
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

                //strzaly wroga
                if (rand() % 60 == 0 && wrogowie.size() > 0){
                    int nr_strzelca = rand() % wrogowie.size();
                    struct pocisk p;
                    p.x= wrogowie[nr_strzelca].x + WROG_SIZE_X/2 - POCISK_SIZE_X/2;
                    p.y = wrogowie[nr_strzelca].y + WROG_SIZE_Y;
                    pociski_wroga.push_back(p);
                }
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
                        czas_do_strzalu = 0;
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
                sprintf(tekst, "Punkty: %d. Życia: %d. Poziom: %d. Czas: %d", wynik, zycie, poziom, (int)czas_gry);
                al_draw_text(font, al_map_rgb(255,255,255), 5, 5 , ALLEGRO_ALIGN_LEFT, tekst);

                for (int i = 0; i < pociski.size(); i++){
                    al_draw_bitmap(pocisk_bitmapa, pociski[i].x, pociski[i].y, 0);
                }

                for (int i = 0; i < pociski_wroga.size(); i++){
                    al_draw_bitmap(pocisk_bitmapa, pociski_wroga[i].x, pociski_wroga[i].y, 0);
                }

                for (int i = 0; i < wrogowie.size(); i++){
                    al_draw_bitmap(wrog_bitmapa, wrogowie[i].x, wrogowie[i].y, 0);
                }

                al_draw_bitmap(gracz_bitmapa, gracz_x, gracz_y, 0);
                if (pokaz_wybuch > 0) {
                    al_draw_bitmap(wybuch_bitmapa, gracz_x-20.0, gracz_y-40.0, 0);
                    pokaz_wybuch--;
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
