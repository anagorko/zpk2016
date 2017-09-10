	//        BIBLIOTEKI		/////////////////
	////////////////////////////////////////////

	#include <allegro5/allegro.h>
	#include <allegro5/allegro_primitives.h>
	#include "allegro5/allegro_image.h"
	#include <allegro5/allegro_font.h>
	#include <allegro5/allegro_ttf.h>

	#include <iostream>
	#include <cmath>
	#include <string>
	#include <fstream>
	#include <vector>
	using namespace std;

	/// zmiennie stale
	const int k_sz = 103;	// szerokosc kafelka
	const int k_wy = 103;	// wysokosc kafelka
	const int liczba_kafelkow = 18; // liczba kafelkow
	const int ekran_sz = (4+2)*k_sz;   // szerokosc ekranu
	const int ekran_w = 4*k_wy;    // wysokosc ekranu
	bool key[ALLEGRO_KEY_MAX];  // wciœniête klawisze
	const int liczba_okien = 3;
	bool wyjdz = false;
	string nicki[5];
	string wyniki[5];

	ALLEGRO_BITMAP* bitmapa[liczba_kafelkow];
	ALLEGRO_FONT* font;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;

	const string wszystkie_pliki[liczba_kafelkow] =
	{
		"kafelki/pusty.png","kafelki/2.png", "kafelki/4.png", "kafelki/8.png", "kafelki/16.png", "kafelki/32.png",
		"kafelki/64.png", "kafelki/128.png", "kafelki/256.png", "kafelki/512.png", "kafelki/1024.png", "kafelki/2048.png",
		"kafelki/menu.png", "kafelki/ekran.png", "kafelki/nick.png", "kafelki/nowa.png", "kafelki/wyjdz.png", "kafelki/dalej.png"
    };

	/////////////////////////////////////////////
	//        KLASY		////////////////////
	////////////////////////////////////////////

	// Klasa przycisk
	class przycisk
	{
	    private:
			int x; // gdzie umiescic przycisk x
			int y; // gdzie umiescic przycisk y
			int w; // wysokość przycisku
			int sz; // szerokość przycisku
			int b; // przycisk myszy aktywujacy
			int plik_przyc; // plik odpowiadający za dany przycisk

			bool o[liczba_okien]; // okienka, w których przycisk dzia³a
	    public:
			przycisk(){};
			przycisk(int _x, int _y, int _w, int _sz, bool _o[liczba_okien], int _plik_przyc, int _b = 1){
				x = _x; y = _y; w = _w; sz = _sz; b = _b; plik_przyc = _plik_przyc;
				int j = 0;
				for(int i = 0; i<liczba_okien; i++){
					o[i] = _o[i];
				}
			}

			bool czy_wybrany(int pb, int px, int py){
				if(b == pb && px>=x && px<=x+sz && py>=y && py<=y+w){
					return true;
				}
				return false;
			}

			void nadaj_przycisk(int _x, int _y, int _w, int _sz, bool _o[liczba_okien], int _plik_przyc, int _b = 1){
				x = _x; y = _y; w = _w; sz = _sz; b = _b; plik_przyc = _plik_przyc;
				int j = 0;
				for(int i = 0; i<liczba_okien; i++){
					o[i] = _o[i];
				}
				return;
			}

			bool czy_w_oknie(int okno){ // sprawdza czy w danym okienku przycisk dziala
				if(o[okno]){
					return true;
				}
				return false;
			}

			void rysuj_przycisk(){

					al_draw_bitmap_region(bitmapa[plik_przyc], 0,
						                  0, sz+34, w+12, x-17, y, 0);
			}


	};
//przyciski
	przycisk tab_przyc[liczba_okien];

//funkcje

	// Wczytuje bitmapy z pliku z kafelkami do tablicy bitmapa[liczba_kafelkow]
	bool wczytanie_bitmapy()
	{
	    for (int i = 0; i < liczba_kafelkow; i++) {
	        bitmapa[i] = al_load_bitmap(wszystkie_pliki[i].c_str());
			    if (!bitmapa[i]) {
	            cerr << "B³¹d podczas wczytywania bitmapy ." << endl;
	            return false;
	        }
	    }
	//czcionka
		font = al_load_ttf_font("phagspa.ttf", 20, 0);

	    return true;
	}

	// wczytanie wynikow z pliku
	void wczytaj_wyniki(){
		ifstream file;
		file.open("wyniki.txt");
		for(int i = 0; i<5; i++){
			getline(file, nicki[i]);
		}
		for(int i = 0; i<5; i++){
			getline(file, wyniki[i]);
		}
		file.close();
	}

	// Zapisanie do pliku
	void zapisz_wyniki(double wynik, ALLEGRO_USTR *nick){
		ofstream file ("wyniki.txt", ios::out | ios::trunc); // open file and clear contents
		int i = 0;
		while(stoi(wyniki[i])>=(int)wynik){
			file << nicki[i] << "\n";
			i++;
		}
		file << al_cstr(nick) << "\n";
		while(i<4){
			file << nicki[i] << "\n";
			i++;
		}
		i = 0;
		while(stoi(wyniki[i])>=(int)wynik){
			file << wyniki[i] << "\n";
			i++;
		}
		file << wynik << "\n";
		while(i<4){
			file << wyniki[i] << "\n";
			i++;
		}
		file.close();
		wczytaj_wyniki();
	}

	// Rysuje plansze
	void rysuj_plansze(int **plansza, double& wynik)
	{
	    al_clear_to_color(al_map_rgb(0,0,0));

	    for (int i = 0; i < 4; i++) {
	        for (int j = 0; j < 4; j++) {
	// gra
	            al_draw_bitmap_region(bitmapa[plansza[j][i]], 0,
	                                  0, k_sz, k_wy, i*k_sz, j*k_sz, 0);	        }
	    }
	// menu
		al_draw_bitmap_region(bitmapa[12], 0,
	                                  0, 2*k_sz, 4*k_wy, 4*k_sz, 0, 0);


		for(int i = 0; i < liczba_okien; i++){

			if(tab_przyc[i].czy_w_oknie(0)){
				tab_przyc[i].rysuj_przycisk();
			}
		}

//aktualny wynik
		al_draw_text(font, al_map_rgb(0,0,0), 4*k_sz+10,k_wy,ALLEGRO_ALIGN_LEFT, "Wynik: ");
		al_draw_text(font, al_map_rgb(0,0,0), 6*k_sz-20,k_wy,ALLEGRO_ALIGN_RIGHT, std::to_string((double)wynik).c_str());
//wypisanie najlepszych wynikow
		al_draw_text(font, al_map_rgb(0,0,0), 4*k_sz+10, 1.3*k_wy,ALLEGRO_ALIGN_LEFT, "Najlepsze wyniki: ");

		for(int i = 0; i<5; i++){
			al_draw_text(font, al_map_rgb(0,0,0), 4*k_sz+10, (1.7+0.3*i)*k_wy,ALLEGRO_ALIGN_LEFT, nicki[i].c_str());
		}

		for(int i = 0; i<5; i++){
			al_draw_text(font, al_map_rgb(0,0,0), 6*k_sz-20, (1.7+0.3*i)*k_wy,ALLEGRO_ALIGN_RIGHT, wyniki[i].c_str());
		}

	}

	// po zakończeniu gry
	void rysuj_koniec(int **plansza, bool naj_wynik, double& wynik, ALLEGRO_USTR *nick = NULL){
		al_draw_bitmap_region(bitmapa[13], 0, 0, 4*k_sz, 4*k_wy, 0, 0, 0); // ekran
		al_draw_text(font, al_map_rgb(0,0,0), 2*k_sz, 1.4*k_wy,ALLEGRO_ALIGN_CENTRE, "KONIEC GRY!");

		// sprawdza, czy osiagnieto jeden z 5 najlepszych wynikow
		if(naj_wynik){

			al_draw_text(font, al_map_rgb(0,0,0), 2*k_sz, 1.8*k_wy,ALLEGRO_ALIGN_CENTRE, "TO JEDEN Z NAJLEPSZYCH WYNIKÓW!");
			al_draw_text(font, al_map_rgb(0,0,0), 2*k_sz, 2.2*k_wy,ALLEGRO_ALIGN_CENTRE, "WPISZ NICK:");
			al_draw_text(font, al_map_rgb(0,0,0), 2*k_sz, 2.6*k_wy, ALLEGRO_ALIGN_CENTRE, al_cstr(nick));
			for(int i = 0; i<liczba_okien; i++){
				if(tab_przyc[i].czy_w_oknie(1)){
					tab_przyc[i].rysuj_przycisk();
				}
			}
		}
		else{
			for(int i = 0; i<liczba_okien; i++){
				if(tab_przyc[i].czy_w_oknie(2)){
					tab_przyc[i].rysuj_przycisk();
				}
			}
		}

	}

	// Tablica nowej gry (pusta)
	int** pusta_plansza()
	    {
	      int** plansza = 0;
	      plansza = new int*[4];

	      for (int i = 0; i < 4; i++)
	      {
	            plansza[i] = new int[4];

	            for (int j = 0; j < 4; j++)
	            {
	                  plansza[i][j] = 0;
	            }
	      }

	      return plansza;
	    }



	// Dodaje kafelek
	int** dodaj_kafelek(int **plansza, double& wynik)
	{
		int puste = 0;
		int liczba_pocz; //losowanie liczby pierwszej (2-90%, 4-10%)
		int pozycja_pocz; //losowanie pozycji dla liczby pierwszej
		int przebyte_kafelki; int wiersz; int kolumna; // potrzebne do okreslenia pozycji nowego kafelka
		int ile_nowych; //nowa gra – dwa, w trakcie – jeden

		// zliczamy puste
		for(int i = 0; i<4; i++){
			for(int j=0; j<4; j++){
				if(plansza[i][j] == 0){
					puste++;
				}
			}
		}

		if(puste == 16){
			ile_nowych = 2;
		}
		else
		{
			ile_nowych = 1;		}

		// Dodawanie kafelka
		for(int k = 0; k<ile_nowych; k++){
			puste = 0;
			for(int i = 0; i<4; i++){
				for(int j=0; j<4; j++){
					if(plansza[i][j] == 0){
						puste++;
					}
				}
			}
			//losowanie liczby
			liczba_pocz = rand() % 10+1;
			if(liczba_pocz!=1){
				liczba_pocz = 1;
			}
			else
			{
				liczba_pocz = 2;
			}
			// losowanie pozycji
			pozycja_pocz = rand() % puste+1;

			// wstawianie kafelka
			przebyte_kafelki = 0;
			wiersz = 0;
			kolumna = 0;

			while(przebyte_kafelki<pozycja_pocz)
			{
				if(plansza[wiersz][kolumna] == 0){
					przebyte_kafelki++;
				}
				if(przebyte_kafelki<pozycja_pocz)
				{
					if(kolumna!=3){
						kolumna++;
					}
					else
					{
						kolumna=0;
						wiersz++;
					}
				}
			}



				plansza[wiersz][kolumna] = liczba_pocz;
			}
		return plansza;
	}


	int** przesun_kafelki(int **plansza, int ruszanie, int& czy_dodac)
	{

		// 0 – gora, 1 – prawo, 2 – dol, 3 – lewo, 4 - nic

	if(ruszanie == 0)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<3;j++)
			{
					if(plansza[j][i] == 0 && plansza[j+1][i]!=0)
					{
						plansza[j][i] = plansza[j+1][i];
						plansza[j+1][i] = 0;
						czy_dodac = 1;
					}
			}
		}
	}
	else if(ruszanie == 1)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=3;j>0;j--)
			{
				if(plansza[i][j] == 0 && plansza[i][j-1]!=0)
				{
						plansza[i][j] = plansza[i][j-1];
						plansza[i][j-1] = 0;
						czy_dodac = 1;
				}
			}
		}
	}
	else if(ruszanie == 3)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<3;j++)
			{
				if(plansza[i][j] == 0 && plansza[i][j+1]!=0)
				{
						plansza[i][j] = plansza[i][j+1];
						plansza[i][j+1] = 0;
						czy_dodac = 1;
				}
			}
		}
	}
	else if(ruszanie == 2)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=3;j>0;j--)
			{
					if(plansza[j][i] == 0 && plansza[j-1][i]!=0)
					{
						plansza[j][i] = plansza[j-1][i];
						plansza[j-1][i] = 0;
						czy_dodac = 1;
					}
			}
		}
	}

		return plansza;
	}

	// laczenie kafelkow
	int** polacz_kafelki(int **plansza, int ruszanie, int& czy_dodac, double& wynik)
	{
		// sprawdzenie czy laczyc
		bool czy_laczyc = 0;


		if(ruszanie == 0)
		{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4-1;j++)
			{
					if(plansza[j][i] !=0 && plansza[j][i]==plansza[j+1][i])
					{
						plansza[j][i]++;
						plansza[j+1][i] = 0;
						czy_dodac = 1;
						czy_laczyc = 1;
						wynik = wynik + pow(2.0, plansza[j][i]/1.0);
					}
			}
		}
		}
		else if(ruszanie == 1)
		{
		for(int i=0;i<4;i++)
		{
			for(int j=3;j>0;j--)
			{
				if(plansza[i][j] != 0 && plansza[i][j] == plansza[i][j-1])
				{
						plansza[i][j]++;
						plansza[i][j-1] = 0;
						czy_dodac = 1; czy_laczyc = 1;
						wynik = wynik + pow(2.0, plansza[i][j]/1.0);
				}
			}
		}
		}
		else if(ruszanie == 3)
		{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<3;j++)
			{
				if(plansza[i][j] != 0 && plansza[i][j] == plansza[i][j+1])
				{
						plansza[i][j]++;
						plansza[i][j+1] = 0;
						czy_dodac = 1; czy_laczyc = 1;
						wynik = wynik + pow(2.0, plansza[i][j]/1.0);
				}
			}
		}
	}
	else if(ruszanie == 2)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=3;j>0;j--)
			{
					if(plansza[j][i] != 0 && plansza[j][i] == plansza[j-1][i])
					{
						plansza[j][i]++;
						plansza[j-1][i] = 0;
						czy_dodac = 1; czy_laczyc = 1;
						wynik = wynik + pow(2.0, plansza[j][i]/1.0);
					}
			}
		}
	}


	plansza = przesun_kafelki(plansza, ruszanie, czy_dodac);

	if(czy_laczyc == 1){

		if(ruszanie!=4){
			rysuj_plansze(plansza, wynik);
			al_flip_display();
			al_rest(0.02);

		}
	}

		return plansza;
	}

	// sprawdzenie czy koniec gry
	bool czy_koniec(int **plansza){

	// nie ma miejsca na nowe kafelki
		for(int i = 0; i<4; i++){
			for(int j = 0; j<4; j++){
				if(plansza[i][j] == 0){
					return 0;
				}
			}
		}

		// rozne wartości obok siebie w pionie

		for(int i = 0; i<3; i++){
			for(int j = 0; j<4; j++){
				if(plansza[i][j] == plansza[i+1][j]){
					return 0;
				}
			}
		}

		// rozne wartości obok siebie w poziomie

		for(int i = 0; i<3; i++){
			for(int j = 0; j<4; j++){
				if(plansza[j][i] == plansza[j][i+1]){
					return 0;
				}
			}
		}

		return 1;
	}

	// Inicjalizacja
	int init()
	{
	    if(!al_init()) {
	        cerr << "B³¹d podczas inicjalizacji allegro." << endl;
	        return -1;
	    }

	    if (!al_init_primitives_addon()) {
	        cerr << "B³¹d podczas inicjalizacji dodatku 'primitives'." << endl;
	        return -1;
	    }

	    if(!al_init_image_addon()) {
	        cerr << "B³¹d podczas inicjalizacji dodatku 'image'." << endl;
	        return -1;
	    }

	    if(!al_install_keyboard()) {
	        cerr << "B³¹d podczas inicjalizacji klawiatury." << endl;
	        return -1;
	    }

		if(!al_install_mouse()) {
	        cerr << "B³¹d podczas inicjalizacji myszy." << endl;
	        return -1;
	    }

	    display = al_create_display(ekran_sz, ekran_w);
	    if(!display) {
	        cerr << "B³¹d podczas inicjalizacji ekranu." << endl;
	        return -1;
	    }

	    event_queue = al_create_event_queue();
	    if(!event_queue) {
	        cerr << "B³¹d podczas inicjalizacji kolejki zdarzeñ." << endl;
	        al_destroy_display(display);
	        return -1;
	    }
		al_init_font_addon(); // initialize the font addon
		al_init_ttf_addon();// initialize the ttf (True Type Font) addon
	    	al_register_event_source(event_queue, al_get_display_event_source(display));
	    	al_register_event_source(event_queue, al_get_keyboard_event_source());
		al_register_event_source(event_queue, al_get_mouse_event_source());
	   	al_clear_to_color(al_map_rgb(0,0,0));
	   	al_flip_display();

	    return 0;
	}

	// nazwanie strzalek
	int ruch(){
		if (key[ALLEGRO_KEY_UP])
		{
			return 0;
	    }
		else if(key[ALLEGRO_KEY_RIGHT]){
			return 1;
		}
		else if(key[ALLEGRO_KEY_DOWN]){
			return 2;
		}
		else if(key[ALLEGRO_KEY_LEFT])
		{
			return 3;
		}
		else
		{
			return 4;
		}
	}
	int main(int argc, char **argv)
	{
		// Inicjalizacja
	    if (init() != 0) {
	        cerr << "Inicjalizacja nie powiod³a siê." << endl;
	        return -1;
	    }

	    if (!wczytanie_bitmapy()) {
	        return -1;
	    }

		// Zmienne w mainie
	   int klucz; // klucz pamieta wcisniete przyciski
	   int czy_dodac; // czy dodac kafel, czyli czy byl jakis ruszanie po wcisnieciu przycisku
	   int ktory_klawisz = 0; //ktory_klawisz został wscisniety 0 - nic 1 - nowa gra, 2 - wyjdz
	   double wynik = 0; // deklaruje wynik
	   int okno = 0; // okno, które działa: gra (okno = 0),
				// koniec gry z najlepszym wynikiem (okno = 1),
				// koniec gry (okno = 2)
		// wpisanie nick’u
			ALLEGRO_USTR *nick = al_ustr_new("");
		// licznik liter – tablice 5 elementowe
			int licznik_liter = 0;

        int** plansza = pusta_plansza();
	   wczytaj_wyniki(); // wczytuje najepsze wyniki

	   // Przyciski
	   // 0 - new game, 1 - wyjdz, 2 - dalej
	   bool tab[liczba_kafelkow] = {true,false,false};
	   bool tab2[liczba_kafelkow] = {false,true,true};
	   tab_przyc[0].nadaj_przycisk(430, 30, 65, 260, tab, 15);
	   tab_przyc[1].nadaj_przycisk(430, 340, 65, 260, tab, 16);
	   tab_przyc[2].nadaj_przycisk(430, 340, 65, 260, tab2, 17);

	   // pierwsze rysowanie
	   rysuj_plansze(plansza, wynik);
	   al_flip_display();

	   // glowna petla
	    while(!wyjdz)
	    {
	    ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(okno == 0){

			czy_dodac = 0;
				if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
					klucz = ev.keyboard.keycode;
					key[klucz] = true;
					int ruszanie = ruch();

					for(int i = 0; i<3;i++){
						plansza = przesun_kafelki(plansza, ruszanie, czy_dodac);
						rysuj_plansze(plansza, wynik);
						al_flip_display();
						al_rest(0.02);
					}

	// laczenie kafelkow
					plansza = polacz_kafelki(plansza, ruszanie, czy_dodac, wynik);
					rysuj_plansze(plansza, wynik);
					al_flip_display();
					al_rest(0.08);

	// dodawanie kafelka
					if(czy_dodac ==1)
					{
						plansza = dodaj_kafelek(plansza, wynik);
						rysuj_plansze(plansza, wynik);
						al_flip_display();
					}

					key[klucz] = false;
				}
				else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
					key[ev.keyboard.keycode] = false;
		// sprawdzenie końca gry i wyniku
					if(czy_koniec(plansza) == true && stoi(wyniki[4])<(int)wynik){
						okno = 1;
						rysuj_plansze(plansza, wynik);
						rysuj_koniec(plansza, true, wynik, nick);
						al_flip_display();
					}
					else if(czy_koniec(plansza) == true){
						okno = 2;
						rysuj_plansze(plansza, wynik);
						rysuj_koniec(plansza, false, wynik, nick);
						al_flip_display();
					}
				}
				else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
					// nowa gra
					if(tab_przyc[0].czy_wybrany(ev.mouse.button,ev.mouse.x,ev.mouse.y)){
						ktory_klawisz = 1;

						rysuj_plansze(plansza, wynik);
						al_flip_display();

					}
					// wyjscie
					if(tab_przyc[1].czy_wybrany(ev.mouse.button,ev.mouse.x,ev.mouse.y)){
						ktory_klawisz = 2;

						rysuj_plansze(plansza, wynik);
						al_flip_display();

					}

				}
				else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
					// wyjście - co się dzieje
					if(ktory_klawisz == 2){
						rysuj_plansze(plansza, wynik);
						al_flip_display();
						al_rest(0.1);
						break;
					}
					// nowa gra – co się dzieje
					if(ktory_klawisz == 1){
						wynik = 0;
						plansza = pusta_plansza();
						plansza = dodaj_kafelek(plansza, wynik);
						rysuj_plansze(plansza, wynik);
						al_flip_display();
						ktory_klawisz = 0;
					}
				}
			}
			else if(okno == 1){

				if(ev.type == ALLEGRO_EVENT_KEY_CHAR){
					if(ev.keyboard.unichar >32 && licznik_liter<5){
						al_ustr_append_chr(nick, ev.keyboard.unichar);
						rysuj_plansze(plansza, wynik);
						rysuj_koniec(plansza, true, wynik, nick);
						al_flip_display();
						licznik_liter++;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && licznik_liter>0){
						al_ustr_truncate(nick, licznik_liter-1);
						rysuj_plansze(plansza, wynik);
						rysuj_koniec(plansza, true, wynik, nick);
						al_flip_display();
						licznik_liter--;
					}
				}
				else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
					// dalej
					if(tab_przyc[2].czy_wybrany(ev.mouse.button,ev.mouse.x,ev.mouse.y)){
						ktory_klawisz = 1;
						rysuj_plansze(plansza, wynik);
						rysuj_koniec(plansza, true, wynik, nick);
						al_flip_display();

					}
				}
				else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
					if(ktory_klawisz == 1){
						zapisz_wyniki(wynik, nick); // zapisanie wyniku do Naj wynikrs
						wynik = 0;
						rysuj_plansze(plansza, wynik);
						rysuj_koniec(plansza, true, wynik, nick);
						al_flip_display();
						al_rest(0.1);
						plansza = pusta_plansza();
						plansza = dodaj_kafelek(plansza, wynik);
						rysuj_plansze(plansza, wynik);
						al_flip_display();
						ktory_klawisz = 0;
						licznik_liter = 0;
						nick = al_ustr_new("");
						okno = 0;
					}
				}
			}
			else if (okno == 2)
			{
				if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
					// wcisniecie dalej
					if(tab_przyc[2].czy_wybrany(ev.mouse.button,ev.mouse.x,ev.mouse.y)){
						ktory_klawisz = 1;
						rysuj_plansze(plansza, wynik);
						rysuj_koniec(plansza, false, wynik);
						al_flip_display();

					}
				}
				else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
					if(ktory_klawisz == 1){
						wynik = 0;
						rysuj_plansze(plansza, wynik);
						rysuj_koniec(plansza, false, wynik);
						al_flip_display();
						al_rest(0.1);
						plansza = pusta_plansza();
						plansza = dodaj_kafelek(plansza, wynik);
						rysuj_plansze(plansza, wynik);
						al_flip_display();
						ktory_klawisz = 0;
						okno = 0;
					}
				}
			}
		}
		return 0;
	}


