#include<iostream>
#include<stdio.h>
#include <fstream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <vector>
using namespace std;

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>



////////////////////////GRA W ŻYCIE////////////////////////////////////////////////////////////////////////////////////////////
////////////Wioletta Osuch & Damian Iwanowski//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEFINICJE KLAS//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int l_pol = 100;  //liczba pól na planszy z wybieraniem żywych komórek

class Punkt
{
public:
    int x, y; // współrzędne x i y punktu

    Punkt() {}
    Punkt(int _x, int _y) { //konstruktor
        x = _x;
        y = _y;
    }
};

class Pole
{
public:
    Punkt srodek; //Punkt wskazujący na środek Pola
    Punkt a, b, c, d; //wierzchołki Pola: a - gorny lewy, b - górny prawy, c - dolny lewy, d - dolny prawy
    int bok_pion, bok_poziom; //długości boków

    Pole() {} //Pusty konstruktor
    Pole(int _srodek_x, int _srodek_y, int _bok_poziom, int _bok_pion) {
        Ustaw(_srodek_x, _srodek_y, _bok_poziom, _bok_pion);
    }

    void Ustaw(int _srodek_x, int _srodek_y, int _bok_poziom, int _bok_pion);// Metoda pozwala ustawić, zmodyfikować wartości zmiennych obiektu
    void Rysuj_Pusty(ALLEGRO_COLOR kolor, int gr_linii);// Metoda pozwala na narysowanie obiektu bez wypełnienia przy zadanym kolorze i grubości linii krawędzi
    void Rysuj_Pelny(ALLEGRO_COLOR kolor);// Metoda pozwala na narysowanie obiektu z wypełnieniem w zadanym kolorze
    void Rysuj_PustyZaokr(ALLEGRO_COLOR kolor, int gr_linii);// Metoda pozwala na narysowanie obiektu z zaokrąglonymi rogami bez wypełnienia przy zadanym kolorze i grubości linii krawędzi
    void Rysuj_PelnyZaokr(ALLEGRO_COLOR kolor);// Metoda pozwala na narysowanie obiektu z zaokrąglonymi rogami z wypełnieniem przy zadanym kolorze
    void Wpisz_Tekst(string tekst, ALLEGRO_FONT *czcionka, ALLEGRO_COLOR kolor);// Metoda pozwala na wpisanie tekstu do obiektu (wyśrodkowany)
    void Wpisz_TekstL(string tekst, ALLEGRO_FONT *czcionka, ALLEGRO_COLOR kolor);// Metoda pozwala na wpisanie tekstu do obiektu (wyrównany do lewej)
    void Wpisz_Liczbe(int liczba, ALLEGRO_FONT *czcionka, ALLEGRO_COLOR kolor);// Metoda pozawalająca na wpisanie liczby (zmiennej typu int) do obiektu
    void Wpisz_TrojkatPelny(ALLEGRO_COLOR kolor, string kierunek);// Metoda pozwala skonstruować "strzałkę" (prostokąt z odpowiednio skierowanym  trójkątem w środku
    bool Klikniety(int x, int y); // Metoda pozwalająca sprawdzić, czy użytkownik kliknął na obiekt
};

class Plansza
{
    public:

    int **Tablica_1;//Dwie tablice, aby przepisywać stan gry w kolejnych rundach
    int **Tablica_2;

    int wys_p, szer_p;//Wysokość i szerokość planszy, na której odbywa się symulacja

    vector <int> ozywa;//Wektory zawierające reguły użytkownika, dla których żywa konórka umiera oraz martwa ożywa
    vector <int> umiera;

    Plansza() {
        wys_p = l_pol; //Konstruktor planszy o odpowiednich wymiarach
        szer_p = l_pol;
    }

    void Pusta_tablica(); //Metoda tworząca dwie puste tablice wypełnione zerami (jedna buforowa)
	bool Dodaj_komorke(int x,int y);// Metoda dodająca żywą komórkę o współrzędnych (x,y)
	bool Czy_zywa(int x,int y);// Metoda sprawdzająca, czy komórka o wspolrzędnych (x,y) jest żywa (czy w Tablicy_1 jest 1)
	int Ile_sasiadow(int x,int y);// Metoda licząca żywych sąsiadow komórki o wspołrzędnych (x,y)
	void Symulacja_podst();//Metoda służąca do symulacji gry w życie z regułami gry wg Conwaya
	void Symulacja_wlasna();//Metoda służąca do symulacji gry w życie z regułami własnymi użytkownika
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GRA W ŻYCIE////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int, char**)
{
    const int h =700; //wysokosc ekranu
    const int w = 700; //szerokosc ekranu

    int L_Rund = 1000; // Liczba rund po ilu kończy się symulacja (w symulacji użytkownika jest ustalana przez użytkownika)
    int Pierwsza_Runda = 1; // Nr rundy, która ma zostać wyświetlona na ekranie (w symulacji użytkownika można wybrać by wyświetlać symulację np. dopiero od 501 rundy(pierwsze 500 rund zostanie przeprowadzonych bez wyświetlania na ekranie))
    int Runda; //Nr obecnie wyświetlanej rundy

    int rozm = 6; //długość boku pojedynczego pola na planszy z wybieraniem żywych komórek
    int lewy_bok = w / 2 - (l_pol * rozm / 2) + rozm / 2; //polozenie lewgo boku (wspolrzędna X) planszy z wybieraniem żywych komórek
    int gora_bok = h / 2 - (l_pol * rozm / 2) + rozm / 2; //polozenie górnego boku (współrzędna Y) planszy z wybieraniem żywych komórek
    bool Tablica[l_pol][l_pol]; //tablica z wartościami prawda/fałsz dla planszy z wybieraniem żywych komórek

    bool Tab_Ozywa[9]; // tablica przechowująca zasady ożywania nieżyjących komórek wprowadzone przez użytkownika
    bool Tab_Umiera[9]; // tablica przechowująca zasady umierania żyjących komórek wprowadzone przez użytkownika


    int szybkosc = 50; //Zmienna pomocnicza do ustalania szybkości symulacji (im większa wartość zmiennej, tym wolniej przebiega symulacja)
    int moc; //Licznik do ustalania prędkości symulacji (zmienna rośnie za każdym tikiem timera, jeśli jego wartość przekroczy wartość zmiennej szybkosc to zostanie wyświetlona kolejna runda, a licznik zostanie wyzerowany i cały proces rozpocznie się od nowa)
    bool Pauza = false; //Zmienna pomocnicza - przyjmuje wartość true, jeśli użytkownik zapauzowal symulację
    bool koniec = false; //Zmienna pomocnicza - przyjmuje wartość true, gdy po wyświetleniu ostatniej rundy (gdy int Runda >= int L_Rund)


    int ekran = 1; // Zmienna pomocnicza do konstrukcji switch...case

    bool rysuj = true; // Zmienna pomocnicza, przyjmuje wartość true, za każdym tikiem timera, a wartość false po każdym odświeżeniu ekranu

    bool Sym_Uzytkownika = false; //Zmienna pomocnicza, przyjmuje wartość true, jeśli została wybrana symulacja użytkownika


    Plansza p; // Zmienna klasy Plansza, z pomocą ktorej wykonywane są obliczenia niezbędne do przeprowadzenia symulacji

    // Poniższe obiekty klasy Pole tworza wszelkie widoczne w okienku napisy i przyciski
    // Pola z ekranu 1
    Pole Tytul(w / 2, 250, 500, 100);
    Pole Autorzy (w / 2+150, 50, 300, 40);
    Pole Przycik_Zagraj (w / 2, 460, 240, 60);
    Pole Przycisk_Zasady (w / 2, 580, 240, 60);

    // Pola z ekranu 2
    Pole Tytul2 (w / 2, 150, 500, 100);
    Pole Autorzy2 (w / 2+150, 50, 300, 40);
    Pole Zasady (w / 2, 200, 240, 80);
    Pole OpisZasad1 (w/35, 250, 25, 25);
    Pole OpisZasad2 (w/35, 270, 25, 25);
    Pole OpisZasad3 (w/35, 300, 25, 25);
    Pole OpisZasad4 (w/35, 330, 25, 25);
    Pole OpisZasad5 (w/35, 350, 25, 25);
    Pole OpisZasad6 (w/35, 380, 25, 25);
    Pole OpisZasad7 (w/35, 410, 25, 25);
    Pole OpisZasad8 (w/35, 430, 25, 25);
    Pole OpisZasad9 (w/35, 450, 25, 25);
    Pole OpisZasad10 (w/35, 480, 25, 25);
    Pole OpisZasad11 (w/35, 510, 25, 25);
    Pole OpisZasad12 (w/35, 530, 25, 25);
    Pole OpisZasad13 (w/35, 550, 25, 25);
    Pole OpisZasad14 (w/35, 570, 25, 25);
    Pole OpisZasad15 (w/35, 590, 25, 25);

    Pole Przycisk_Wroc (w / 2, h - 50, 240, 60);

    // Pola z ekranu 3
    Pole Przycisk_SymPodst (w / 2, 300, 500, 60);
    Pole Przycisk_SymUz (w / 2, 400, 500, 60);

    // Pola z ekranu 4
    Pole WskazZywe (w / 2, 20, 300, 40);
    Pole Przycisk_Dalej (w / 2 + 45, h - 25, 80, 40); // też w kolejnych ekranach
    Pole Przycisk_Wroc2 (w / 2 - 45, h - 25, 80, 40); // też w kolejnych ekranach
    Pole Przycisk_Kwadrat[l_pol][l_pol];

    // Pola z ekranu 5
    Pole Tytul3 (w / 2, 40, 400, 60); //też w Menu_Uzytkownik_2
    Pole Podtytul (w / 2, 100, 300, 40); //też w Menu_Uzytkownik_2
    Pole Pytanie1 (w / 2, 300, 300, 40);
    Pole Pytanie2 (w / 2, 450, 300, 40);
    Pole Przycisk_LewoLRund (w / 2 - 60, 350, 40, 40);
    Pole Przycisk_PrawoLRund (w / 2 + 60, 350, 40, 40);
    Pole Przycisk_Lewo1Runda (w / 2 - 60, 500, 40, 40);
    Pole Przycisk_Prawo1Runda (w / 2 + 60, 500, 40, 40);
    Pole LiczbaRund (w / 2, 350, 60, 40);
    Pole PierwszaRunda (w / 2, 500, 60, 40);

    // Pola z ekranu 6
    Pole Napis1 (w / 2, 290, 300, 40);
    Pole Napis2 (w / 2, 370, 300, 40);
    Pole Napis3 (w / 2, 510, 300, 40);
    Pole Napis4 (w / 2, 590, 300, 40);
    Pole Przycisk_RegulaOzywa[9];
    Pole Przycisk_RegulaUmiera[9];

    // Pola z ekranu 7
    Pole Tytul4 (w / 2, 20, 200, 40);
    Pole Napis_AktRunda (90, 20, 120, 40);
    Pole AktualnaRunda (190, 20, 60, 40);
    Pole Napis_MaxRund (w - 140, 20, 120, 40);
    Pole MaxRund (w - 35, 20, 60, 40);
    Pole Przycisk_Menu (100, h - 25, 160, 40);
    Pole Przycisk_Pauza (w / 2, h - 25, 80, 40);
    Pole Napis_Szybkosc (w - 180, h - 25, 80, 40);
    Pole Przycisk_Wolniej (w - 110, h - 25, 40, 40);
    Pole Przycisk_Szybciej (w - 60, h - 25, 40, 40);


    // Zainicjalizoanie Allegro
    al_init();
    al_init_primitives_addon();
    al_install_mouse();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY *okienko = al_create_display(w, h); // stworzenie okienka o wymiarach w, h
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue(); // stworzenie kolejki zdarzeń
    ALLEGRO_TIMER *timer = al_create_timer(1.0/60); // stworzenie timera (tik, co 1/60 sekundy)

    //Czcionki w różnych rozmiarach
    ALLEGRO_FONT *Font_S = al_load_ttf_font("arialn.ttf",16,0 );
    ALLEGRO_FONT *Font_M = al_load_ttf_font("arialn.ttf",20,0 );
    ALLEGRO_FONT *Font_L = al_load_ttf_font("arialn.ttf",36,0 );
    ALLEGRO_FONT *Font_XL = al_load_ttf_font("arialn.ttf",60,0 );
    ALLEGRO_FONT *Font_XXL = al_load_ttf_font("arialn.ttf",80,0 );

    // Kolory
    ALLEGRO_COLOR kol_tl=al_map_rgb(0, 0, 51);
    ALLEGRO_COLOR kol_logo_nap1=al_map_rgb(200, 200, 200);
    ALLEGRO_COLOR kol_logo_nap2=al_map_rgb(200, 200, 200);
    ALLEGRO_COLOR kol_przycisk_tl=al_map_rgb(160, 160, 160);
    ALLEGRO_COLOR kol_przycisk_nap=al_map_rgb(0, 0, 102);
    ALLEGRO_COLOR kol_wybor;


    ALLEGRO_EVENT ev;

    al_register_event_source(event_queue, al_get_display_event_source(okienko)); //Zarejestrowanie zdarzeń okienkowych do kolejki (potrzebne by móc zamykać program za pomocą krzyżyka (x)
    al_register_event_source(event_queue, al_get_timer_event_source(timer)); //Zarejestrowanie timera do kolejki (bez tego nie było by zdarzeń timera w kolejce)
    al_register_event_source(event_queue, al_get_mouse_event_source()); //Zarejestrowanie zdarzeń myszy (bez tego nie było by w kolejce informacji o poruszaniu myszą oraz o wciśnięciu przycisków myszy)

    al_start_timer(timer); // Uruchomienie timera


    // Ustawienie wartości zmiennych dla tablic typu Pole
    // dla tablicy Przycisk_Kwadrat [l_pol][l_pol]
    for (int i = 0; i < l_pol; i++) {
        for (int j = 0; j < l_pol; j++) {
            Przycisk_Kwadrat[i][j].Ustaw(lewy_bok + i * rozm, gora_bok + j * rozm, rozm, rozm);
        }
    }

    // dla tablicy Przycisk_RegulaOzywa [9]
    for (int i = 0; i < 9; i++) {
        Przycisk_RegulaOzywa[i].Ustaw(w / 2 - 220 + i * 50, 330, 40, 40);
    }

    // dla tablicy Przycisk_RegulaUmiera [9]
    for (int i = 0; i < 9; i++) {
        Przycisk_RegulaUmiera[i].Ustaw(w / 2 - 220 + i * 50, 550, 40, 40);
    }


    //Resetuję tablicę z wybieraniem żywych komórek na starcie
    for (int i = 0; i < l_pol; i++) {
        for (int j = 0; j < l_pol; j++) {
            Tablica[i][j]=false;
        }
    }

    //Resetuję obie tablice z wybieraniem zasad
    for (int i = 0; i < 9; i++) {
        Tab_Ozywa[i]=false;
        Tab_Umiera[i]=false;
    }

    //Pętla programu
    while (true) {

        al_flip_display();

        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER) {
         rysuj = true;
        }

        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
             ekran = 0;
             break;
        }

        switch (ekran) {

        case 1:

            if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if (Przycik_Zagraj.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    ekran = 3;
                    break;
                }
                else if (Przycisk_Zasady.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    ekran = 2;
                    break;
                }
            }

            if(rysuj && al_is_event_queue_empty(event_queue)) {
                rysuj = false;

                al_clear_to_color(kol_tl);

            Tytul.Wpisz_Tekst("GRA W ŻYCIE", Font_XXL, kol_logo_nap1);

            Autorzy.Wpisz_Tekst("W. Osuch & D. Iwanowski", Font_M, kol_logo_nap2);

            Przycik_Zagraj.Rysuj_PelnyZaokr(kol_przycisk_tl);
            Przycik_Zagraj.Wpisz_Tekst("ZAGRAJ", Font_L, kol_przycisk_nap);

            Przycisk_Zasady.Rysuj_PelnyZaokr(kol_przycisk_tl);
            Przycisk_Zasady.Wpisz_Tekst("ZASADY GRY", Font_L, kol_przycisk_nap);
            }
            break;


        case 2:

            if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if (Przycisk_Wroc.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    ekran = 1;
                    break;
                }
            }

            if(rysuj && al_is_event_queue_empty(event_queue)) {
                rysuj = false;

                al_clear_to_color(kol_tl);

               Autorzy2.Wpisz_Tekst("W. Osuch & D. Iwanowski", Font_M, kol_logo_nap2);

            Tytul2.Wpisz_Tekst("GRA W ŻYCIE", Font_XL, kol_logo_nap2);

            Zasady.Wpisz_Tekst("ZASADY GRY", Font_L, kol_logo_nap1);

            OpisZasad1.Wpisz_TekstL("Gra w życie (Life, The game of life) – jeden z pierwszych i najbardziej znanych przykładów " , Font_S, kol_logo_nap1);
            OpisZasad2.Wpisz_TekstL("automatu komórkowego wymyślony w roku 1970 przez brytyjskiego matematyka Johna Conwaya" , Font_S, kol_logo_nap1);
            OpisZasad3.Wpisz_TekstL("Reguły gry według Conwaya:" , Font_M, kol_logo_nap1);
            OpisZasad4.Wpisz_TekstL("Martwa komórka, która ma dokładnie 3 żywych sąsiadów, staje się żywa w następnym etapie " , Font_S, kol_logo_nap1);
            OpisZasad5.Wpisz_TekstL("Żywa komórka z 2 albo 3 żywymi sąsiadami pozostaje żywa; przy innej liczbie sąsiadów umiera" , Font_S, kol_logo_nap1);
            OpisZasad6.Wpisz_TekstL("Własne reguły gry:" , Font_M, kol_logo_nap1);
            OpisZasad7.Wpisz_TekstL("Określamy, dla ilu żywych sąsiadów martwa komórka staje się żywa w następnym etapie " , Font_S, kol_logo_nap1);
            OpisZasad8.Wpisz_TekstL("Określamy,dla ilu sąsiadów żywa komórka pozostanie nadal żywa" , Font_S, kol_logo_nap1);
            OpisZasad9.Wpisz_TekstL("Dla innej ilości żywych sąsiadów komórka umrze" , Font_S, kol_logo_nap1);
            OpisZasad10.Wpisz_TekstL("Przeprowadzenie symulacji:" , Font_M, kol_logo_nap1);
            OpisZasad11.Wpisz_TekstL("Niniejsza gra pozwala na przeprowadzenie symulacji gry w życie dla zasad Conwaya" , Font_S, kol_logo_nap1);
            OpisZasad12.Wpisz_TekstL("oraz zasad własnych użytkownika. Użytkownik może także wybrać początkowe rozmieszczenie" , Font_S, kol_logo_nap1);
            OpisZasad13.Wpisz_TekstL("żywych komórek a także ilośc rund symulacji i od której rundy symulacja ma być wyświetlona." , Font_S, kol_logo_nap1);
            OpisZasad14.Wpisz_TekstL("Istnieje także możliwość regulowania szybkości symulacji za pomocą przycisków '+' i '-'." , Font_S, kol_logo_nap1);
            OpisZasad15.Wpisz_TekstL("Użytkownik może także zatrzymać symulację i ponownie ją wznowić." , Font_S, kol_logo_nap1);

            Przycisk_Wroc.Rysuj_PelnyZaokr(kol_przycisk_tl);
            Przycisk_Wroc.Wpisz_Tekst("WRÓĆ", Font_L, kol_przycisk_nap);


            }
            break;


        case 3:

            if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if (Przycisk_SymPodst.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    //jeśli wybrano sym. podstawową
                    Sym_Uzytkownika = false;
                    ekran = 4;
                    break;
                }
                else if (Przycisk_SymUz.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    //jęsli wybrano sym. użytkonika
                    Sym_Uzytkownika = true;
                    ekran = 4;
                    break;
                }
                else if (Przycisk_Wroc.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    // Jeśli kliknięto "Wróć"
                    ekran = 1;
                    break;
                }
            }

            if(rysuj && al_is_event_queue_empty(event_queue)) {
                rysuj = false;

            al_clear_to_color(kol_tl);

            Autorzy2.Wpisz_Tekst("W. Osuch & D. Iwanowski", Font_M, kol_logo_nap2);

            Tytul2.Wpisz_Tekst("GRA W ŻYCIE", Font_XL, kol_logo_nap1);

            Przycisk_SymPodst.Rysuj_PelnyZaokr(kol_przycisk_tl);
            Przycisk_SymPodst.Wpisz_Tekst("ZASADY CONWAYA", Font_L, kol_przycisk_nap);

            Przycisk_SymUz.Rysuj_PelnyZaokr(kol_przycisk_tl);
            Przycisk_SymUz.Wpisz_Tekst("ZASADY UŻYTKOWNIKA", Font_L, kol_przycisk_nap);
            }
            break;


        case 4:

            if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                for (int i = 0; i < l_pol; i++) {
                    for (int j = 0; j < l_pol; j++) {
                        if (Przycisk_Kwadrat[i][j].Klikniety(ev.mouse.x, ev.mouse.y)) {
                            Tablica[i][j] = (!Tablica[i][j]);
                        }
                    }
                }
                if (Przycisk_Dalej.Klikniety(ev.mouse.x, ev.mouse.y)) {
                        ekran = 5;
                        break;
                    }


                else if (Przycisk_Wroc2.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    ekran = 3;
                    break;
                }
            }

            if(rysuj && al_is_event_queue_empty(event_queue)) {
                rysuj = false;

                al_clear_to_color(kol_tl);

                WskazZywe.Wpisz_Tekst("Wskaż żywe komórki", Font_L, kol_logo_nap1);

                Przycisk_Dalej.Rysuj_PelnyZaokr(kol_przycisk_tl);
                Przycisk_Dalej.Wpisz_Tekst("DALEJ", Font_M, kol_przycisk_nap);

                Przycisk_Wroc2.Rysuj_PelnyZaokr(kol_przycisk_tl);
                Przycisk_Wroc2.Wpisz_Tekst("WRÓĆ", Font_M, kol_przycisk_nap);

                for (int i = 0; i < l_pol; i++) {
                    for (int j = 0; j < l_pol; j++) {
                        if (Tablica[i][j] == false) {
                            kol_wybor = al_map_rgb(200, 0, 0);
                        }
                        else if (Tablica[i][j] == true) {
                            kol_wybor = al_map_rgb(0, 200, 0);
                        }
                        Przycisk_Kwadrat[i][j].Rysuj_Pelny(kol_wybor);
                        Przycisk_Kwadrat[i][j].Rysuj_Pusty(al_map_rgb(0, 0, 200), 1);
                    }
                }
            }
            break;


        case 5:

            if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                // Jeśli kliknięto lewą strzałkę dla pierwsej opcji
                if (Przycisk_LewoLRund.Klikniety(ev.mouse.x, ev.mouse.y) && (L_Rund - 100 > Pierwsza_Runda) ) {
                    L_Rund -= 100;
                }
                // Jeśli kliknięto prawą strzałkę dla pierwsej opcji
                else if (Przycisk_PrawoLRund.Klikniety(ev.mouse.x, ev.mouse.y) && (L_Rund + 100 < 100000)) {
                    L_Rund += 100;
                }

                // Jeśli kliknięto lewą strzałkę dla drugiej opcji
                if (Przycisk_Lewo1Runda.Klikniety(ev.mouse.x, ev.mouse.y) && (Pierwsza_Runda==100)) {
                    Pierwsza_Runda -= 99;
                }
                else if (Przycisk_Lewo1Runda.Klikniety(ev.mouse.x, ev.mouse.y) &&  (Pierwsza_Runda - 100 > 0 )) {
                    Pierwsza_Runda -= 100;
                }

                // Jeśli kliknięto prawą strzałkę dla drugiej opcji
                else if (Przycisk_Prawo1Runda.Klikniety(ev.mouse.x, ev.mouse.y) && (Pierwsza_Runda==1) ) {
                    Pierwsza_Runda += 99;
                }


                else if (Przycisk_Prawo1Runda.Klikniety(ev.mouse.x, ev.mouse.y) && (Pierwsza_Runda + 100 < L_Rund)) {
                    Pierwsza_Runda += 100;
                }

                //Jeśli kliknięto przycisk "Dalej" + symulacja użytkownika
                else if (Przycisk_Dalej.Klikniety(ev.mouse.x, ev.mouse.y) && (Sym_Uzytkownika==TRUE)) {
                    ekran = 6;
                    break;
                }
                //Jeżli klknięto przycisk "Dalej" + symulacja podstawowa
                 else if (Przycisk_Dalej.Klikniety(ev.mouse.x, ev.mouse.y) && (Sym_Uzytkownika==FALSE)) {
                    ekran = 7;
                    Runda = 1;
                    moc = 0;
                    break;
                }


                //Jeśli kliknięto przycisk "Wróć"
                else if (Przycisk_Wroc2.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    ekran = 4;
                    break;
                }
            }

            if(rysuj && al_is_event_queue_empty(event_queue)) {
                rysuj = false;

                al_clear_to_color(kol_tl);


                Tytul3.Wpisz_Tekst("GRA W ŻYCIE", Font_XL, kol_logo_nap1);

                if (Sym_Uzytkownika==TRUE){
                Podtytul.Wpisz_Tekst("Symulacja użytkownika", Font_L, kol_logo_nap1);
                }

                else if (Sym_Uzytkownika==FALSE){
                Podtytul.Wpisz_Tekst("Symulacja podstawowa", Font_L, kol_logo_nap1);
                }

                // Pierwsza opcja (ile rund ma trwać symulacja)
                Pytanie1.Wpisz_Tekst("Ile rund ma trwać symulacja?", Font_M, kol_logo_nap1);

                // Strzałka w lewo (zmniejszenie zmiennej L_Rund o 100)
                Przycisk_LewoLRund.Rysuj_Pusty(kol_przycisk_tl, 2);
                Przycisk_LewoLRund.Wpisz_TrojkatPelny(kol_przycisk_tl, "L");

                // kwadrat z obecną wartością L_Rund
                LiczbaRund.Rysuj_Pusty(kol_przycisk_tl, 2);
                LiczbaRund.Wpisz_Liczbe(L_Rund, Font_M, kol_przycisk_tl);

                // Strzałka w prawo (zwiększenie zmiennej L_Rund o 100)
                Przycisk_PrawoLRund.Rysuj_Pusty(kol_przycisk_tl, 2);
                Przycisk_PrawoLRund.Wpisz_TrojkatPelny(kol_przycisk_tl, "P");


                // Druga opcja (od której rundy ma zostać wyświetlona symulacja)
                Pytanie2.Wpisz_Tekst("Od której rundy ma zostać wyświetlona symulacja?", Font_M, kol_logo_nap1);

                // Strzałka w lewo (zmniejszenie zmiennej Pierwsza_Runda o 100)
                Przycisk_Lewo1Runda.Rysuj_Pusty(kol_przycisk_tl, 2);
                Przycisk_Lewo1Runda.Wpisz_TrojkatPelny(kol_przycisk_tl, "L");

                // kwadrat z obecną wartością Pierwsza_Runda
                PierwszaRunda.Rysuj_Pusty(kol_przycisk_tl, 2);
                PierwszaRunda.Wpisz_Liczbe(Pierwsza_Runda, Font_M, kol_przycisk_tl);

                // Strzałka w prawo (zwiększenie zmiennej Pierwsza_Runda o 100)
                Przycisk_Prawo1Runda.Rysuj_Pusty(kol_przycisk_tl, 2);
                Przycisk_Prawo1Runda.Wpisz_TrojkatPelny(kol_przycisk_tl, "P");


                //Przycisk "Dalej"
                Przycisk_Dalej.Rysuj_Pelny(kol_przycisk_tl);
                Przycisk_Dalej.Wpisz_Tekst("DALEJ", Font_M, kol_przycisk_nap);

                //Przycisk "Wróć"
                Przycisk_Wroc2.Rysuj_Pelny(kol_przycisk_tl);
                Przycisk_Wroc2.Wpisz_Tekst("WRÓĆ", Font_M, kol_przycisk_nap);
            }
            break;


        case 6:

            if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                // Jeśli kliknięto któryś z przycisków reguł ożywania
                for (int i = 0; i < 9; i++) {
                    if (Przycisk_RegulaOzywa[i].Klikniety(ev.mouse.x, ev.mouse.y) && Tab_Umiera[i] == false) {
                        Tab_Ozywa[i] = (!Tab_Ozywa[i]);
                    }
                }

                // Jeśli kliknięto któryś z przycisków reguł umierania
                for (int i = 0; i < 9; i++) {
                    if (Przycisk_RegulaUmiera[i].Klikniety(ev.mouse.x, ev.mouse.y) && Tab_Ozywa[i] == false) {
                        Tab_Umiera[i] = (!Tab_Umiera[i]);
                    }
                }

                // Jeśli kliknięto przycisk "Dalej"
                if (Przycisk_Dalej.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    // Usunięcie wszystkich elementów wektora z poprzedniego uruchomienia
                    p.ozywa.clear();
                    p.umiera.clear();
                    for (int i = 0; i < 9; i++) {
                        if (Tab_Ozywa[i] == true) {
                            p.ozywa.push_back(i);
                        }
                        if (Tab_Umiera[i] == true) {
                            p.umiera.push_back(i);
                        }
                    }
                    ekran = 7;
                    Runda = 1;
                    moc = 0;
                    break;
                }
                // Jeśli kliknięto przycisk "Wróć"
                if (Przycisk_Wroc2.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    ekran = 5;
                    break;
                }
            }

            if(rysuj && al_is_event_queue_empty(event_queue)) {
                rysuj = false;

                al_clear_to_color(kol_tl);

                Tytul3.Wpisz_Tekst("GRA W ŻYCIE", Font_XL, kol_logo_nap1);

                Podtytul.Wpisz_Tekst("Nowe zasady gry", Font_L, kol_logo_nap1);

                // Pierwsza zasada "Komórka ożywa jeśli wokół niej jest:"
                Napis1.Wpisz_Tekst("Komórka ożywa jeśli wokół niej jest:", Font_M, kol_logo_nap1);

                for (int i = 0; i < 9; i++) {
                    if (Tab_Umiera[i] == true) {
                        kol_wybor=kol_tl;
                    }
                    else if (Tab_Umiera[i] == false && Tab_Ozywa[i] == false) {
                        kol_wybor=al_map_rgb(0, 0, 150);
                    }
                    else if (Tab_Umiera[i] == false && Tab_Ozywa[i] == true) {
                        kol_wybor=al_map_rgb(0, 150, 0);
                    }

                    Przycisk_RegulaOzywa[i].Rysuj_Pusty(kol_wybor, 2);
                    Przycisk_RegulaOzywa[i].Wpisz_Liczbe(i, Font_M, kol_wybor);
                }
                Napis2.Wpisz_Tekst("żywych komórek.", Font_M,kol_logo_nap1);

                // Druga zasada "Komórka umiera jeśli wokół niej jest:"
                Napis3.Wpisz_Tekst("Komórka umiera jeśli wokół niej jest:", Font_M, kol_logo_nap1);

                for (int i = 0; i < 9; i++) {
                    if (Tab_Ozywa[i] == true) {
                        kol_wybor=kol_tl;
                    }
                    else if (Tab_Ozywa[i] == false && Tab_Umiera[i] == false) {
                        kol_wybor=al_map_rgb(0, 0, 150);
                    }
                    else if (Tab_Ozywa[i] == false && Tab_Umiera[i] == true) {
                        kol_wybor=al_map_rgb(0, 150, 0);
                    }

                    Przycisk_RegulaUmiera[i].Rysuj_Pusty(kol_wybor, 2);
                    Przycisk_RegulaUmiera[i].Wpisz_Liczbe(i, Font_M, kol_wybor);
                }
                Napis4.Wpisz_Tekst("żywych komórek.", Font_M, kol_logo_nap1);

                //Przycisk "Dalej"
                Przycisk_Dalej.Rysuj_Pelny(kol_przycisk_tl);
                Przycisk_Dalej.Wpisz_Tekst("DALEJ", Font_M, kol_przycisk_nap);

                //Przycisk "Wróć"
                Przycisk_Wroc2.Rysuj_Pelny(kol_przycisk_tl);
                Przycisk_Wroc2.Wpisz_Tekst("WRÓĆ", Font_M, kol_przycisk_nap);
            }
            break;


        case 7:

            if (Runda == 1) {
            //Zainicjalizowanie symulacji
            // poniższe instrukcje wykonywane są tylko raz - na początku symulacji
                p.Pusta_tablica();

                for (int i = 0; i < l_pol; i++) {
                    for (int j = 0; j < l_pol; j++) {
                        if (Tablica[i][j]==true){
                            p.Dodaj_komorke(i, j);
                        }
                    }
                }



                while (Runda < Pierwsza_Runda) {
                    if (Sym_Uzytkownika==TRUE) {
                        p.Symulacja_wlasna();
                    }
                    else {
                        p.Symulacja_podst();
                    }
                    Runda++;
                }
            }


            if(ev.type == ALLEGRO_EVENT_TIMER) {
                if (Pauza == false) moc++;
            }

            else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if (Przycisk_Menu.Klikniety(ev.mouse.x, ev.mouse.y)) {
                    ekran = 1;
                }
                else if (Przycisk_Pauza.Klikniety(ev.mouse.x, ev.mouse.y) && koniec == false) {
                    Pauza = (!Pauza);
                }

                else if (Przycisk_Szybciej.Klikniety(ev.mouse.x, ev.mouse.y) && szybkosc >= 5 && koniec == false) {
                    szybkosc -= 5;
                }
                else if (Przycisk_Wolniej.Klikniety(ev.mouse.x, ev.mouse.y) && szybkosc <= 115 && koniec == false) {
                    szybkosc += 5;
                }
            }

            if(rysuj && al_is_event_queue_empty(event_queue)) {
                rysuj = false;

                al_clear_to_color(kol_tl);

                Tytul4.Wpisz_Tekst("SYMULACJA", Font_L, kol_logo_nap1);

                Napis_AktRunda.Wpisz_Tekst("Aktualna runda:", Font_M, kol_logo_nap1);
                AktualnaRunda.Wpisz_Liczbe(Runda, Font_M, kol_logo_nap1);

                Napis_MaxRund.Wpisz_Tekst("Ostatnia runda:", Font_M, kol_logo_nap1);
                MaxRund.Wpisz_Liczbe(L_Rund, Font_M, kol_logo_nap1);


                Przycisk_Menu.Rysuj_PelnyZaokr(kol_przycisk_tl);
                Przycisk_Menu.Wpisz_Tekst("Wróć do menu",Font_M, kol_przycisk_nap);

                Przycisk_Pauza.Rysuj_PelnyZaokr(kol_przycisk_tl);
                if (Pauza == true && koniec == false) {
                    Przycisk_Pauza.Wpisz_Tekst("Wznów", Font_M, kol_przycisk_nap);
                }
                else if (Pauza == false && koniec == false){
                    Przycisk_Pauza.Wpisz_Tekst("Pauza", Font_M, kol_przycisk_nap);
                }
                else {
                    Przycisk_Pauza.Wpisz_Tekst("Koniec", Font_M, kol_przycisk_nap);
                }

                Napis_Szybkosc.Wpisz_Tekst("Szybkość:", Font_M, kol_logo_nap1);

                Przycisk_Wolniej.Rysuj_Pelny(kol_przycisk_tl);
                Przycisk_Wolniej.Wpisz_Tekst("-", Font_M, kol_przycisk_nap);

                Przycisk_Szybciej.Rysuj_Pelny(kol_przycisk_tl);
                Przycisk_Szybciej.Wpisz_Tekst("+", Font_M, kol_przycisk_nap);

                for (int i = 0; i < l_pol; i++) {
                    for (int j = 0; j < l_pol; j++) {
                        if (p.Tablica_1[i][j]==0) {
                            kol_wybor = al_map_rgb(200, 0, 0);
                        }
                        else {
                            kol_wybor = al_map_rgb(0, 200, 0);
                        }
                        Przycisk_Kwadrat[i][j].Rysuj_Pelny(kol_wybor);
                        Przycisk_Kwadrat[i][j].Rysuj_Pusty(al_map_rgb(0, 0, 200), 1);
                    }
                }

                if (moc > szybkosc && koniec == false && Pauza == false) {
                // Przejście do następnej rundy symulacji
                    moc=0;
                    if (Sym_Uzytkownika) {
                        p.Symulacja_wlasna();
                    }
                    else {
                        p.Symulacja_podst();
                    }

                    if (Runda >= L_Rund) {
                        koniec = true;
                    } else {
                        Runda ++;
                    }
                }
            }
            break;


        case 0:

            al_destroy_timer(timer);
            al_destroy_display(okienko);
            al_destroy_event_queue(event_queue);
            exit(0);
        }
    }

   return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Metody klas//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Metody klasy Pole

// Metoda pozwala ustawić, zmodyfikować wartości zmiennych obiektu
void Pole::Ustaw(int _srodek_x, int _srodek_y, int _bok_poziom, int _bok_pion) {
    srodek.x = _srodek_x;//Współrzędne punktu cięzkości
    srodek.y = _srodek_y;
    bok_poziom = _bok_poziom;//Długości boków
    bok_pion = _bok_pion;
    a.x = srodek.x - bok_poziom / 2; //Wyznaczenie współrzędnych wierzchołków
    a.y = srodek.y - bok_pion / 2;
    b.x = srodek.x + bok_poziom / 2;
    b.y = srodek.y - bok_pion / 2;
    c.x = srodek.x - bok_poziom / 2;
    c.y = srodek.y + bok_pion / 2;
    d.x = srodek.x + bok_poziom / 2;
    d.y = srodek.y + bok_pion / 2;
}

// Metoda pozwala na narysowanie obiektu bez wypełnienia przy zadanym kolorze
// i grubości linii krawędzi
void Pole::Rysuj_Pusty(ALLEGRO_COLOR kolor, int gr_linii) {
    al_draw_rectangle(a.x, a.y, d.x, d.y, kolor, gr_linii);
}

// Metoda pozwala na narysowanie obiektu z wypełnieniem w zadanym kolorze
void Pole::Rysuj_Pelny(ALLEGRO_COLOR kolor) {
    al_draw_filled_rectangle(a.x, a.y, d.x, d.y, kolor);
}

// Metoda pozwala na narysowanie obiektu bez wypełnienia z zaokrąglonymi rogami,
//przy zadanym kolorze i grubości linii krawędzi
void Pole::Rysuj_PustyZaokr(ALLEGRO_COLOR kolor, int gr_linii) {
    al_draw_rounded_rectangle(a.x, a.y, d.x, d.y, 20, 20 , kolor, gr_linii);
}

// Metoda pozwala na narysowanie obiektu z zaokrąglonymi rogami,
// z wypełnieniem w zadanym kolorze
void Pole::Rysuj_PelnyZaokr(ALLEGRO_COLOR kolor) {
    al_draw_filled_rounded_rectangle(a.x, a.y, d.x, d.y, 20, 20 , kolor);
}

// Metoda pozwala na wpisanie tekstu do obiektu
void Pole::Wpisz_Tekst(string tekst, ALLEGRO_FONT *czcionka, ALLEGRO_COLOR kolor) {
    al_draw_text(czcionka, kolor, srodek.x, a.y + 10, ALLEGRO_ALIGN_CENTRE, tekst.c_str());
}

// Metoda pozwala na wpisanie tekstu do obiektu - wyrównany do lewej
void Pole::Wpisz_TekstL(string tekst, ALLEGRO_FONT *czcionka, ALLEGRO_COLOR kolor) {
    al_draw_text(czcionka, kolor, srodek.x, a.y + 10, ALLEGRO_ALIGN_LEFT, tekst.c_str());
}

// Metoda pozwala na wpisanie liczby (zmiennej typu int) do obiektu
void Pole::Wpisz_Liczbe(int liczba, ALLEGRO_FONT *czcionka, ALLEGRO_COLOR kolor) {
    al_draw_textf(czcionka, kolor, srodek.x, a.y + 10, ALLEGRO_ALIGN_CENTRE, "%d", liczba);
}

// Metoda pozwala skonstruować "strzałkę" (prostokąt z odpowiednio skierowanym
// trójkątem w środku
void Pole::Wpisz_TrojkatPelny(ALLEGRO_COLOR kolor, string kierunek) {
    if (kierunek == "L") {
        al_draw_filled_triangle(a.x + 5, srodek.y, b.x - 5, b.y + 5, d.x - 5, d.y - 5, kolor);
    }
    else if (kierunek == "P") {
        al_draw_filled_triangle(a.x + 5, a.y + 5, c.x + 5, c.y - 5, b.x - 5, srodek.y, kolor);
    }
    else if (kierunek == "G") {
        al_draw_filled_triangle(srodek.x, a.y + 5, c.x + 5, c.y - 5, d.x - 5, d.y - 5, kolor);
    }
    else if (kierunek == "D") {
        al_draw_filled_triangle(a.x + 5, a.y + 5, b.x - 5, b.y + 5, srodek.x, c.y - 5, kolor);
    }
}

// Metoda pozwala na sprawdzenie czy zmienne x i y znajdują się w obrębie obiektu
// Jeśli podstawimy pod x i y współrzędne kursora w momencie kliknięcia możemy
// dzięki tej metodzie sprawdzić czy użytkownik kliknął na obiekt
bool Pole::Klikniety(int x, int y) {
    if (x > a.x && x <  d.x && y > a.y && y < d.y) {
        return true;
    }
    else  {
        return false;
    }
}


//Metody klasy Plansza

//Metoda służąca do tworzenia dwóch tablicwypełnionych zerami
// Dwie, żeby jedna była buforowa i umożliwiala przejście do kolejnego etapu
void Plansza::Pusta_tablica()
{
	Tablica_1=new int*[szer_p];
	Tablica_2=new int*[szer_p];
	for(int i=0;i<szer_p;i++)
	{
		Tablica_1[i]=new int[wys_p];
		Tablica_2[i]=new int[wys_p];
		for(int j=0;j<wys_p;j++)
		{
			Tablica_1[i][j]=0;
			Tablica_2[i][j]=0;
		}
	}
}

// Metoda dodajaca zywa komorke o wspolrzednych (x,y)
bool Plansza::Dodaj_komorke(int x,int y)
{
	if(x<0||y<0||x>=szer_p||y>=wys_p) //Warunki, żeby komórka mieściła się w planszy
	{
		return false;
	}
	Tablica_1[x][y]=1; //Dodaję żywą komórkę, czyli 1 w tablicy
    return 1; //bez tego blad...
}

// Metoda sprawdzająca, czy komórka o wspolrzędnych (x,y) jest żywa (czy w Tablicy_1 jest 1)
bool Plansza::Czy_zywa(int x, int y){
    if (x<0 || x>=szer_p) { return 0; }
    if (y<0 || y>=wys_p) { return 0; }
    else return (Tablica_1[x][y]==1); //Czy w Tablicy_1 w punkcie o wspołrzędnych (x,y) jest żywa kómorka? TRUE/FALSE
}

// Metoda licząca żywych sąsiadow komórki o wspołrzędnych (x,y)
int Plansza::Ile_sasiadow(int x,int y)
{
int sasiedzi=0; //Zmienna pomocnicza licząca żywych sąsiadów

sasiedzi =  Czy_zywa(x-1,y-1) //sprawdzamy dla każdego z 8 sąsiadów
          + Czy_zywa(x,y-1)
          + Czy_zywa(x+1,y-1)
          + Czy_zywa(x+1,y)
          + Czy_zywa(x+1,y+1)
          + Czy_zywa(x,y+1)
          + Czy_zywa(x-1,y+1)
          + Czy_zywa(x-1,y);

    return sasiedzi;
}



//Metoda służąca do symulacji gry w życie z regułami gry wg Conwaya,
//czyli martwa komórka, która ma dokładnie 3 żywych sąsiadów, staje się żywa w następnej jednostce czasu (rodzi się),
//a żywa komórka z 2 albo 3 żywymi sąsiadami pozostaje nadal żywa; przy innej liczbie sąsiadów umiera
void Plansza::Symulacja_podst(){
    int licz;
    for (int x=0; x<szer_p; x++){
        for (int y=0; y<wys_p; y++) { //Dla wszystkich komórek martwych w planszy sprawdzamy, czy jakaś ożyje
            if(Tablica_1[x][y]==0){
                licz = Ile_sasiadow(x,y);
                if(licz==3)
                    Tablica_2[x][y]=1;
            }

            if(Tablica_1[x][y]==1){
                licz = Ile_sasiadow(x,y);
                if(licz==2 || licz ==3) //Dla wszystkich komórek żywych na planszy sprawdzamy, czy jakaś umrze
                    Tablica_2[x][y]=1;
                else
                    Tablica_2[x][y]=0;
            }
        }
    }
    for (int x=0; x<szer_p; x++){
        for (int y=0; y<wys_p; y++){ //Przepisujemy stan gry z tablicy 1 na 2
            Tablica_1[x][y]=Tablica_2[x][y];
        }

    }
}

//Metoda służąca do symulacji gry w życie z regułami własnymi użytkownika
//Użytkownik może wybrać reguły, dla których martwa komórka ożyje oraz  dla których żywa umrze

void Plansza::Symulacja_wlasna(){
    int licz;
    for (int x=0; x<szer_p; x++){
        for (int y=0; y<wys_p; y++) {
            if(Tablica_1[x][y]==0){
                licz = Ile_sasiadow(x,y);
                for(int j=0; j<ozywa.size();j++){
                    if(licz==ozywa[j]) {
                        Tablica_2[x][y]=1; break;}
                }

            }

            if(Tablica_1[x][y]==1){
                licz = Ile_sasiadow(x,y);
                for(int j=0; j<umiera.size();j++){
                    if(licz==umiera[j]) {
                            Tablica_2[x][y]=0; break;}
                }
           }
        }
    }
    for (int x=0; x<szer_p; x++){
        for (int y=0; y<wys_p; y++){
            Tablica_1[x][y]=Tablica_2[x][y];
        }

    }
}

