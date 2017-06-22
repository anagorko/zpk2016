/////////////////////////////////////////////
//        BIBLIOTEKI		/////////////////
////////////////////////////////////////////

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <iostream>
#include <cmath> // do potegowania
#include <string> // do stringow
#include <fstream> // do plikow
#include <vector> // do klasy przycisk
using namespace std;

/////////////////////////////////////////////
//        GLOBALNE		////////////////////
////////////////////////////////////////////

const int k_sz = 151;	// szerokoœæ (piksele) kafelka
const int k_wy = 151;	// wysokoœæ (piksele) kafelka

const int liczba_kafelkow = 92; // liczba kafelkow :)

const int screen_w = (4+2)*k_sz;   // szerokoœæ ekranu (screen width)
const int screen_h = 4*k_wy;    // wysokoœæ ekranu (screen height)

bool key[ALLEGRO_KEY_MAX];  // wciœniête klawisze

const int liczba_okienek = 3; // liczba okienek gry (potrzebne do przyciskow)
 
bool wyjdz = false;

string names[5]; // najlepsi - nazwy
string results[5]; // najlepsi - wyniki

ALLEGRO_BITMAP* bitmapa[liczba_kafelkow];
ALLEGRO_FONT* font;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;

const string plik_z_kafelkiem[liczba_kafelkow] =
{
	// 0 - 17 zwykle kafelki
	// 18 - 35 rozwijanie dwojki i czworki
	// 36 - 83 animacja zlaczenia
	// 84 plansza menu
	// 85 - 88 oraz 90 - 91 klawisze przyciskow
	// 89 plansza przy koncu gry
    "kafelki/t0.png", "kafelki/t1.png", "kafelki/t2.png", "kafelki/t3.png", "kafelki/t4.png", "kafelki/t5.png",
	"kafelki/t6.png", "kafelki/t7.png", "kafelki/t8.png", "kafelki/t9.png", "kafelki/t10.png", "kafelki/t11.png",
	"kafelki/t12.png", "kafelki/t13.png", "kafelki/t14.png", "kafelki/t15.png", "kafelki/t16.png", "kafelki/t17.png", //17
	"kafelki/rozwijanie/roz11.png", "kafelki/rozwijanie/roz21.png", "kafelki/rozwijanie/roz12.png", "kafelki/rozwijanie/roz22.png",
	"kafelki/rozwijanie/roz13.png", "kafelki/rozwijanie/roz23.png", "kafelki/rozwijanie/roz14.png", "kafelki/rozwijanie/roz24.png",
	"kafelki/rozwijanie/roz15.png", "kafelki/rozwijanie/roz25.png", "kafelki/rozwijanie/roz16.png", "kafelki/rozwijanie/roz26.png",
	"kafelki/rozwijanie/roz17.png", "kafelki/rozwijanie/roz27.png", "kafelki/rozwijanie/roz18.png", "kafelki/rozwijanie/roz28.png",
	"kafelki/rozwijanie/roz19.png", "kafelki/rozwijanie/roz29.png", 
	//35
	"kafelki/laczenie/lacz21.png", "kafelki/laczenie/lacz31.png", "kafelki/laczenie/lacz41.png", "kafelki/laczenie/lacz51.png",
	"kafelki/laczenie/lacz61.png", "kafelki/laczenie/lacz71.png", "kafelki/laczenie/lacz81.png", "kafelki/laczenie/lacz91.png",
	"kafelki/laczenie/lacz101.png", "kafelki/laczenie/lacz111.png", "kafelki/laczenie/lacz121.png", "kafelki/laczenie/lacz131.png",
	"kafelki/laczenie/lacz141.png", "kafelki/laczenie/lacz151.png", "kafelki/laczenie/lacz161.png", "kafelki/laczenie/lacz171.png",
	//51
	"kafelki/laczenie/lacz22.png", "kafelki/laczenie/lacz32.png", "kafelki/laczenie/lacz42.png", "kafelki/laczenie/lacz52.png",
	"kafelki/laczenie/lacz62.png", "kafelki/laczenie/lacz72.png", "kafelki/laczenie/lacz82.png", "kafelki/laczenie/lacz92.png",
	"kafelki/laczenie/lacz102.png", "kafelki/laczenie/lacz112.png", "kafelki/laczenie/lacz122.png", "kafelki/laczenie/lacz132.png",
	"kafelki/laczenie/lacz142.png", "kafelki/laczenie/lacz152.png", "kafelki/laczenie/lacz162.png", "kafelki/laczenie/lacz172.png",
	//67
	"kafelki/laczenie/lacz23.png", "kafelki/laczenie/lacz33.png", "kafelki/laczenie/lacz43.png", "kafelki/laczenie/lacz53.png",
	"kafelki/laczenie/lacz63.png", "kafelki/laczenie/lacz73.png", "kafelki/laczenie/lacz83.png", "kafelki/laczenie/lacz93.png",
	"kafelki/laczenie/lacz103.png", "kafelki/laczenie/lacz113.png", "kafelki/laczenie/lacz123.png", "kafelki/laczenie/lacz133.png",
	"kafelki/laczenie/lacz143.png", "kafelki/laczenie/lacz153.png", "kafelki/laczenie/lacz163.png", "kafelki/laczenie/lacz173.png",
	//83
	"kafelki/menu.png", //84
	"kafelki/ngame.png", "kafelki/ngame_down.png", //86
	"kafelki/exit.png", "kafelki/exit_down.png", //88
	"kafelki/end_screen.png", //89
	"kafelki/dalej.png", "kafelki/dalej_down.png" //90, 91
};

/////////////////////////////////////////////
//        KLASY		////////////////////
////////////////////////////////////////////

// Klasa przycisk
class przycisk
{
    private:
		int x; // x coord
		int y; // y coord
		int h; // wysokosc
		int w; // szerokosc
		int b; // przycisk myszy aktywujacy
		int stan; // stan 0 - niewcisniety, 1 - wcisniety
		int bitmap_0; // bitmapa odpowiadajaca za niewcisniety
		int bitmap_1; // bitmapa odpowiadajaca za wcisniety
		bool o[liczba_okienek]; // okienka, w których przycisk dzia³a
    public:
		przycisk(){};
		przycisk(int _x, int _y, int _h, int _w, bool _o[liczba_okienek], int _bitmap_0, int _bitmap_1, int stan = 0, int _b = 1){
			x = _x; y = _y; h = _h; w = _w; b = _b; bitmap_0 = _bitmap_0; bitmap_1 = _bitmap_1;
			int j = 0;
			for(int i = 0; i<liczba_okienek; i++){
				o[i] = _o[i];
			}
		}

		bool czy_wcisniety(int pb, int px, int py){ 
			if(b == pb && px>=x && px<=x+w && py>=y && py<=y+h){
				return true;
			}
			return false;
		}

		void nadaj_przycisk(int _x, int _y, int _h, int _w, bool _o[liczba_okienek], int _bitmap_0, int _bitmap_1, int stan = 0, int _b = 1){
			x = _x; y = _y; h = _h; w = _w; b = _b; bitmap_0 = _bitmap_0; bitmap_1 = _bitmap_1;
			int j = 0;
			for(int i = 0; i<liczba_okienek; i++){
				o[i] = _o[i];
			}
			return;
		}

		bool czy_okienko(int okienko){ // sprawdza czy w danym okienku przycisk dziala
			if(o[okienko]){
				return true;
			}
			return false;
		}

		void rysuj_przycisk(){
			if(stan ==0){
				al_draw_bitmap_region(bitmapa[bitmap_0], 0, 
					                  0, w+34, h+12, x-17, y, 0);
			}
			else{
				al_draw_bitmap_region(bitmapa[bitmap_1], 0, 
					                  0, w+34, h+12, x-17, y, 0);
			}
		}

		void zmien_stan(int s){
			stan = s;
		}

};

przycisk tab_przyc[liczba_okienek]; // Tablica przyciskow - globalnie dla uproszczenia


/////////////////////////////////////////////
//        FUNKCJE		////////////////////
////////////////////////////////////////////


// Wczytuje bitmapy z pliku z kafelkami do tablicy bitmapa[liczba_kafelkow]
bool przygotuj_bitmapy()
{
    for (int i = 0; i < liczba_kafelkow; i++) {
        bitmapa[i] = al_load_bitmap(plik_z_kafelkiem[i].c_str());
		    if (!bitmapa[i]) {
            cerr << "B³¹d podczas wczytywania bitmapy ." << endl;
            return false;
        }
    }
    
	font = al_load_ttf_font("Marlboro.ttf", 40, 0);

    return true;
}

// Wczytuje najlepsze wyniki z pliku "high_scores.txt" i zapisuje
// w tablicach stringow names[5] oraz results[5]
void wczytaj_high_scores(){

	ifstream file;
	file.open("high_scores.txt");

	for(int i = 0; i<5; i++){
		getline(file, names[i]);
	}

	for(int i = 0; i<5; i++){
		getline(file, results[i]);
	}

	file.close();


}

// Zapisuje wynik do pliku z najlepszymi wynikami
void zapisz_high_score(double score, ALLEGRO_USTR *nick){

	ofstream file ("high_scores.txt", ios::out, ios::trunc); // open file and clear contents

	int i = 0;
	while(stoi(results[i])>=(int)score){
		file << names[i] << "\n";
		i++;
	}
	file << al_cstr(nick) << "\n";
	while(i<4){
		file << names[i] << "\n";
		i++;
	}
	i = 0;
	while(stoi(results[i])>=(int)score){
		file << results[i] << "\n";
		i++;
	}
	file << score << "\n";
	while(i<4){
		file << results[i] << "\n";
		i++;
	}
	file.close();
	wczytaj_high_scores();
}

// Rysuje plansze z kafelkami i menu
void rysuj_plansze(int **plansza, double& score)
{
    al_clear_to_color(al_map_rgb(0,0,0));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            al_draw_bitmap_region(bitmapa[plansza[j][i]], 0, 
                                  0, k_sz, k_wy, i*k_sz, j*k_sz, 0); //kostki gry
        }
    }

	al_draw_bitmap_region(bitmapa[84], 0, 
                                  0, 2*k_sz, 4*k_wy, 4*k_sz, 0, 0); //menu plansza


	for(int i = 0; i < liczba_okienek; i++){
	
		if(tab_przyc[i].czy_okienko(0)){
			tab_przyc[i].rysuj_przycisk();
		}
	}

	//al_draw_bitmap_region(bitmapa[ng_st], 0, 
                                  //0, 2*k_sz, 0.5*k_wy, 4*k_sz+5, 5, 0); //new game button

	//al_draw_bitmap_region(bitmapa[ex_st], 0, 
                                  //0, 2*k_sz, 0.5*k_wy, 4*k_sz+5, 3.5*k_wy-5, 0); //exit button

	al_draw_text(font, al_map_rgb(0,0,0), 4*k_sz+5, 0.7*k_wy,ALLEGRO_ALIGN_LEFT, "Wynik: "); //Wynik
	al_draw_text(font, al_map_rgb(0,0,0), 6*k_sz-20, 0.7*k_wy,ALLEGRO_ALIGN_RIGHT, std::to_string((long double)score).c_str());

	al_draw_text(font, al_map_rgb(0,0,0), 4*k_sz+5, 1.3*k_wy,ALLEGRO_ALIGN_LEFT, "Najlepsi w historii: "); //High Scorers
	
	for(int i = 0; i<5; i++){
		al_draw_text(font, al_map_rgb(0,0,0), 4*k_sz+10, (1.7+0.3*i)*k_wy,ALLEGRO_ALIGN_LEFT, names[i].c_str()); //Ich imiona
	}

	for(int i = 0; i<5; i++){
		al_draw_text(font, al_map_rgb(0,0,0), 6*k_sz-20, (1.7+0.3*i)*k_wy,ALLEGRO_ALIGN_RIGHT, results[i].c_str()); //Ich wyniki
	}
	
	
	
}

// Dorysowuje do powyzszego wszystko co dzieje sie na koncu
void rysuj_koniec(int **plansza, bool high_score, double& score, ALLEGRO_USTR *nick = NULL){
	al_draw_bitmap_region(bitmapa[89], 0, 0, 4*k_sz, 4*k_wy, 0, 0, 0); // polprzezroczysta plansza
	al_draw_text(font, al_map_rgb(0,0,0), 2*k_sz, 1.4*k_wy,ALLEGRO_ALIGN_CENTRE, "KONIEC GRY!"); //KONIEC GRY				
	
	// sprawdza, czy osiagnieto jeden z 5 najlepszych wynikow
	if(high_score){
	
		al_draw_text(font, al_map_rgb(0,0,0), 2*k_sz, 1.8*k_wy,ALLEGRO_ALIGN_CENTRE, "GRATULACJE - NAJLEPSZY WYNIK");
		al_draw_text(font, al_map_rgb(0,0,0), 2*k_sz, 2.2*k_wy,ALLEGRO_ALIGN_CENTRE, "WPISZ NICK:");
		al_draw_text(font, al_map_rgb(0,0,0), 2*k_sz, 2.6*k_wy, ALLEGRO_ALIGN_CENTRE, al_cstr(nick));
		for(int i = 0; i<liczba_okienek; i++){
			if(tab_przyc[i].czy_okienko(1)){
				tab_przyc[i].rysuj_przycisk();
			}
		}
	}
	else{
		for(int i = 0; i<liczba_okienek; i++){
			if(tab_przyc[i].czy_okienko(2)){
				tab_przyc[i].rysuj_przycisk();
			}
		}
	}

}

// Tworzy tablice 4x4 z samymi zerami. Gotowa do wypelnienia
int** przygotuj_plansze_start()
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

// Wczytuje plansze i wynik z pliku tekstowego (wczytanie zapisanej gry)
int** wczytaj_plansze(double& score){

	string przeczytane;
	ifstream file;
	file.open("saved_board.txt");
	int** plansza = przygotuj_plansze_start(); 
	
	for(int i = 0; i<4;i++){
		for(int j = 0;j<4;j++){
			getline(file, przeczytane);
			plansza[i][j] = stoi(przeczytane);
		}
	}
	getline(file, przeczytane);
	score = stoi(przeczytane);
	file.close();

	return plansza;
}

// Zapisuje aktualna rozgrywke po wcisnieciu przycisku WYJDZ
void zapisz_plansze(int **plansza, double score){
	ofstream file ("saved_board.txt", ios::out, ios::trunc); // open file and clear contents

	for(int i = 0; i<4; i++){
		for(int j = 0; j<4; j++){
			file << plansza[i][j] << "\n";
		}
	}
	file << score << "\n";
}

// Dodaje kafelek (rozwija po efektywnym przesunieciu)
int** dodaj_kafelek(int **plansza, double& score)
{
	int liczba_pustych = 0;
	int losowanie_pocz_val; //bedziemy losowac wartosc 1(2) lub 2(4) z prawd 9:1
	int losowanie_pocz_pos; //losowac bedziemy tez pozycje
	int przebyte_kafelki; int wiersz; int kolumna; // potrzebne do okreslenia pozycji nowego kafelka
	int ile_dodac; //Jesli nowa gra to dwa, jesli efektywne przesuniecie to jeden

	// zliczamy puste kafelki na planszy
	for(int i = 0; i<4; i++){
		for(int j=0; j<4; j++){
			if(plansza[i][j] == 0){
				liczba_pustych++;
			}
		}
	}

	if(liczba_pustych == 16){
		ile_dodac = 2; //plansza jest pusta, zatem to nowa gra
	}
	else
	{
		ile_dodac = 1; // na planszy sa juz kafelki - efektywne przesuniecie
	}

	// Dodawanie pojedynczego kafelka
	for(int k = 0; k<ile_dodac; k++){
		liczba_pustych = 0;
		for(int i = 0; i<4; i++){
			for(int j=0; j<4; j++){
				if(plansza[i][j] == 0){
					liczba_pustych++;
				}
			}
		}
		// los wartosci
		losowanie_pocz_val = rand() % 10+1;
		if(losowanie_pocz_val!=1){
			losowanie_pocz_val = 1;
		}
		else
		{
			losowanie_pocz_val = 2;
		}
		// los pozycji (wzglednie)
		losowanie_pocz_pos = rand() % liczba_pustych+1;

		// wstawianie kafelka
		przebyte_kafelki = 0;
		wiersz = 0;
		kolumna = 0;
		
		while(przebyte_kafelki<losowanie_pocz_pos)
		{
			if(plansza[wiersz][kolumna] == 0){
				przebyte_kafelki++;
			}
			if(przebyte_kafelki<losowanie_pocz_pos)
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
		
		// Animacja rozwiniecia kafelka
		for(int i = 0; i < 9; i++){	
			plansza[wiersz][kolumna] = losowanie_pocz_val+17+2*i;
			rysuj_plansze(plansza, score);
			al_flip_display();
			al_rest(0.015);
		}
			
			
			plansza[wiersz][kolumna] = losowanie_pocz_val;
		}
	return plansza;
}

// Przesuwa kafelki po ruchu, ale jeszcze nie laczy
int** przesun_puste_kafelki(int **plansza, int ruszyl, int& czy_mam_dodac_kafel)
{

	// ruszyl = 0	w gore
	// ruszyl = 1	w prawo
	// ruszyl = 2	w dol
	// ruszyl = 3	w lewo
	// ruszyl = 4	brak eventu

if(ruszyl == 0)
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<3;j++)
		{
				if(plansza[j][i] == 0 && plansza[j+1][i]!=0)
				{
					plansza[j][i] = plansza[j+1][i];
					plansza[j+1][i] = 0;
					czy_mam_dodac_kafel = 1;
				}
		}
	}
}
else if(ruszyl == 1)
{
	for(int i=0;i<4;i++)
	{
		for(int j=3;j>0;j--)
		{
			if(plansza[i][j] == 0 && plansza[i][j-1]!=0)
			{
					plansza[i][j] = plansza[i][j-1];
					plansza[i][j-1] = 0;
					czy_mam_dodac_kafel = 1;
			}
		}
	}
}
else if(ruszyl == 3)
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(plansza[i][j] == 0 && plansza[i][j+1]!=0)
			{
					plansza[i][j] = plansza[i][j+1];
					plansza[i][j+1] = 0;
					czy_mam_dodac_kafel = 1;
			}
		}
	}
}
else if(ruszyl == 2)
{
	for(int i=0;i<4;i++)
	{
		for(int j=3;j>0;j--)
		{
				if(plansza[j][i] == 0 && plansza[j-1][i]!=0)
				{
					plansza[j][i] = plansza[j-1][i];
					plansza[j-1][i] = 0;
					czy_mam_dodac_kafel = 1;
				}
		}
	}
}

	return plansza;
}

// Laczy kafelki w wieksze
int** polacz_kafelki(int **plansza, int ruszyl, int& czy_mam_dodac_kafel, double& score)
{
	// plansze majace pamietac animacje 1,2,3 oraz indykator czy animowac

	int** plansza_ani1 = przygotuj_plansze_start(); 
	int** plansza_ani2 = przygotuj_plansze_start();
	int** plansza_ani3 = przygotuj_plansze_start();
	bool cokolwiek_zlaczone = 0;

	// plansze animacyjne zaczynaja od planszy
	for(int i = 0; i<4; i++){
		for(int j = 0; j<4; j++){
			plansza_ani1[i][j] = plansza[i][j];
			plansza_ani2[i][j] = plansza[i][j];
			plansza_ani3[i][j] = plansza[i][j];
		}
	}

	if(ruszyl == 0)
	{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4-1;j++)
		{
				if(plansza[j][i] !=0 && plansza[j][i]==plansza[j+1][i])
				{
					plansza[j][i]++;
					plansza_ani1[j][i] = plansza_ani1[j][i]+35; //relatywnie do pozycji w bitmapa[i]
					plansza_ani2[j][i] = plansza_ani2[j][i]+51;
					plansza_ani3[j][i] = plansza_ani3[j][i]+67;
					plansza[j+1][i] = 0;
					plansza_ani1[j+1][i] = 0;
					plansza_ani2[j+1][i] = 0;
					plansza_ani3[j+1][i] = 0;
					czy_mam_dodac_kafel = 1;
					cokolwiek_zlaczone = 1;
					score = score + pow(2.0, plansza[j][i]/1.0);
				}
		}
	}
	}
	else if(ruszyl == 1)
	{
	for(int i=0;i<4;i++)
	{
		for(int j=3;j>0;j--)
		{
			if(plansza[i][j] != 0 && plansza[i][j] == plansza[i][j-1])
			{
					plansza[i][j]++;
					plansza_ani1[i][j] = plansza_ani1[i][j]+35;
					plansza_ani2[i][j] = plansza_ani2[i][j]+51;
					plansza_ani3[i][j] = plansza_ani3[i][j]+67;
					plansza[i][j-1] = 0;
					plansza_ani1[i][j-1] = 0;
					plansza_ani2[i][j-1] = 0;
					plansza_ani3[i][j-1] = 0;
					czy_mam_dodac_kafel = 1; cokolwiek_zlaczone = 1;
					score = score + pow(2.0, plansza[i][j]/1.0);
			}
		}
	}
	}
	else if(ruszyl == 3)
	{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(plansza[i][j] != 0 && plansza[i][j] == plansza[i][j+1])
			{
					plansza[i][j]++;
					plansza_ani1[i][j] = plansza_ani1[i][j]+35;
					plansza_ani2[i][j] = plansza_ani2[i][j]+51;
					plansza_ani3[i][j] = plansza_ani3[i][j]+67;
					plansza[i][j+1] = 0;
					plansza_ani1[i][j+1] = 0;
					plansza_ani2[i][j+1] = 0;
					plansza_ani3[i][j+1] = 0;
					czy_mam_dodac_kafel = 1; cokolwiek_zlaczone = 1;
					score = score + pow(2.0, plansza[i][j]/1.0);
			}
		}
	}
}
else if(ruszyl == 2)
{
	for(int i=0;i<4;i++)
	{
		for(int j=3;j>0;j--)
		{
				if(plansza[j][i] != 0 && plansza[j][i] == plansza[j-1][i])
				{
					plansza[j][i]++;
					plansza_ani1[j][i] = plansza_ani1[j][i]+35;
					plansza_ani2[j][i] = plansza_ani2[j][i]+51;
					plansza_ani3[j][i] = plansza_ani3[j][i]+67;
					plansza[j-1][i] = 0;
					plansza_ani1[j-1][i] = 0;
					plansza_ani2[j-1][i] = 0;
					plansza_ani3[j-1][i] = 0;
					czy_mam_dodac_kafel = 1; cokolwiek_zlaczone = 1;
					score = score + pow(2.0, plansza[j][i]/1.0);
				}
		}
	}
}

	// Animacja zlaczenia

plansza = przesun_puste_kafelki(plansza, ruszyl, czy_mam_dodac_kafel);


if(cokolwiek_zlaczone == 1){
	plansza_ani1 = przesun_puste_kafelki(plansza_ani1, ruszyl, czy_mam_dodac_kafel);
	plansza_ani2 = przesun_puste_kafelki(plansza_ani2, ruszyl, czy_mam_dodac_kafel);
	plansza_ani3 = przesun_puste_kafelki(plansza_ani3, ruszyl, czy_mam_dodac_kafel);

	if(ruszyl!=4){
		rysuj_plansze(plansza, score);
		al_flip_display();
		al_rest(0.02);

		rysuj_plansze(plansza_ani1, score);
		al_flip_display();
		al_rest(0.02);

		rysuj_plansze(plansza_ani2, score);
		al_flip_display();
		al_rest(0.02);

		rysuj_plansze(plansza_ani3, score);
		al_flip_display();
		al_rest(0.03);

		rysuj_plansze(plansza_ani2, score);
		al_flip_display();
		al_rest(0.02);

		rysuj_plansze(plansza_ani1, score);
		al_flip_display();
		al_rest(0.02);
	}
}

	return plansza;
}

// Sprawdza, czy sa jeszcze mozliwe ruchy
bool sprawdz_koniec_gry(int **plansza){

	// Sprawdza, czy gra skonczona
	// 1. Wszystkie kafelki musza byc niezerowe


	for(int i = 0; i<4; i++){
		for(int j = 0; j<4; j++){
			if(plansza[i][j] == 0){
				return 0;
			}
		}
	}

	// 2. Dwa takie same kafelki nie moga byc obok siebie w pionie

	for(int i = 0; i<3; i++){
		for(int j = 0; j<4; j++){
			if(plansza[i][j] == plansza[i+1][j]){
				return 0;
			}
		}
	}

	// 3. Dwa takie same kafelki nie moga byc obok siebie w poziomie

	for(int i = 0; i<3; i++){
		for(int j = 0; j<4; j++){
			if(plansza[j][i] == plansza[j][i+1]){
				return 0;
			}
		}
	}

	return 1;
}

// Standardowa inicjalizacja klawiatury, myszy, kolejki zdarzen, dodatkow, czcionek
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
 
    display = al_create_display(screen_w, screen_h);
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

// Dla uproszczenia, ktora strzalka wcisnieta
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

    if (!przygotuj_bitmapy()) {
        return -1;
    }
   
	// Zmienne w mainie
   int klucz; // klucz pamieta wcisniete przyciski
   int czy_mam_dodac_kafel; // czy dodac kafel, czyli czy byl jakis ruch po wcisnieciu przycisku
   int status = 0; //status wcisniecia przyciskow na ekranie 0 - nic 1 - nowa gra, 2 - wyjdz
   double score = 0; // deklaruje wynik
   int okienko = 0; // które okienko w³aœnie dzia³a tj. czy jest gra (okienko = 0), 
					// czy jest koniec gry z najlepszym wynikiem (okienko = 1),
					// czy jest po prostu koniec gry (okienko = 2)
   // Zmienne w okienko = 1
	// Wpisywanie przez uzytkownika nazwy
		ALLEGRO_USTR *nick = al_ustr_new("");
	// Liczymy litery, nie chcemy wiecej ni¿ 5 ze wzgledu na miejsce
		int licznik_liter = 0;

   int** plansza = wczytaj_plansze(score); // wczytuje zapisana tablice (gre)
   wczytaj_high_scores(); // wczytuje najepsze wyniki

   // Przyciski
   // 0 - new game, 1 - wyjdz, 2 - dalej
   bool tab[liczba_kafelkow] = {true,false,false}; 
   bool tab2[liczba_kafelkow] = {false,true,true};
   tab_przyc[0].nadaj_przycisk(625, 15, 55, 260, tab, 85, 86);
   tab_przyc[1].nadaj_przycisk(625, 530, 55, 260, tab, 87, 88);
   tab_przyc[2].nadaj_przycisk(170, 425, 55, 260, tab2, 90, 91);

   // pierwsze rysowanie
   rysuj_plansze(plansza, score);
   al_flip_display(); 

   // glowna petla gry
    while(!wyjdz)
    {
    ALLEGRO_EVENT ev;
	al_wait_for_event(event_queue, &ev);
	if(okienko == 0){

		czy_mam_dodac_kafel = 0;
			if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
				klucz = ev.keyboard.keycode;
				key[klucz] = true;
				int ruszyl = ruch();

				// Trzy przesuniecia (maks jest trzy)
				for(int i = 0; i<3;i++){
					plansza = przesun_puste_kafelki(plansza, ruszyl, czy_mam_dodac_kafel);
					rysuj_plansze(plansza, score);
					al_flip_display();
					al_rest(0.02);
				}

				// Potem polaczenie kafelkow (w srodku jest jeszcze jedno przesuniecie, podczas
				// animacji zlaczenia
				plansza = polacz_kafelki(plansza, ruszyl, czy_mam_dodac_kafel, score);
				rysuj_plansze(plansza, score);
				al_flip_display();
				al_rest(0.08);
			
				// Dodawanie kafelka (o ile ruch byl efektywny)
				if(czy_mam_dodac_kafel ==1)
				{
					plansza = dodaj_kafelek(plansza, score);
					rysuj_plansze(plansza, score);
					al_flip_display();
				}

				key[klucz] = false;
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
				key[ev.keyboard.keycode] = false;
				// Sprawdzenie czy gra sie skonczyla i czy high scores
				if(sprawdz_koniec_gry(plansza) == true && stoi(results[4])<(int)score){
					okienko = 1;
					rysuj_plansze(plansza, score);
					rysuj_koniec(plansza, true, score, nick);
					al_flip_display();
				}
				else if(sprawdz_koniec_gry(plansza) == true){
					okienko = 2;
					rysuj_plansze(plansza, score);
					rysuj_koniec(plansza, false, score, nick);
					al_flip_display();
				}
			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				// wcisniecie new game button
				if(tab_przyc[0].czy_wcisniety(ev.mouse.button,ev.mouse.x,ev.mouse.y)){
					status = 1;
					tab_przyc[0].zmien_stan(1);
					rysuj_plansze(plansza, score);
					al_flip_display();
					tab_przyc[0].zmien_stan(0);
				}
				// wcisniecie klawisza exit
				if(tab_przyc[1].czy_wcisniety(ev.mouse.button,ev.mouse.x,ev.mouse.y)){
					status = 2;
					tab_przyc[1].zmien_stan(1);
					rysuj_plansze(plansza, score);
					al_flip_display();
					tab_przyc[1].zmien_stan(0);
				}

			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
				// co robimy gdy exit
				if(status == 2){
					rysuj_plansze(plansza, score);
					al_flip_display();
					al_rest(0.1);
					zapisz_plansze(plansza, score);
					break;
				}
				// co robimy gdy new game
				if(status == 1){
					score = 0;
					plansza = przygotuj_plansze_start();
					plansza = dodaj_kafelek(plansza, score);
					rysuj_plansze(plansza, score);
					al_flip_display();
					status = 0;
				}
			}
		}
		else if(okienko == 1){			
							
			if(ev.type == ALLEGRO_EVENT_KEY_CHAR){
				if(ev.keyboard.unichar >32 && licznik_liter<5){
					al_ustr_append_chr(nick, ev.keyboard.unichar);
					rysuj_plansze(plansza, score);
					rysuj_koniec(plansza, true, score, nick);
					al_flip_display();
					licznik_liter++;
				}
				else if(ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && licznik_liter>0){
					al_ustr_truncate(nick, licznik_liter-1);
					rysuj_plansze(plansza, score);
					rysuj_koniec(plansza, true, score, nick);
					al_flip_display();
					licznik_liter--;		
				}
			}
			else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				// wcisniecie dalej
				if(tab_przyc[2].czy_wcisniety(ev.mouse.button,ev.mouse.x,ev.mouse.y)){
					status = 1;
					tab_przyc[2].zmien_stan(1);
					rysuj_plansze(plansza, score);
					rysuj_koniec(plansza, true, score, nick);
					al_flip_display();
					tab_przyc[2].zmien_stan(0);
				}
			}
			else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
				if(status == 1){
					zapisz_high_score(score, nick); // zapisanie wyniku do High scorers
					score = 0;
					rysuj_plansze(plansza, score);
					rysuj_koniec(plansza, true, score, nick);
					al_flip_display();
					al_rest(0.1);
					// Z automatu zaczynam nowa gre
					plansza = przygotuj_plansze_start();
					plansza = dodaj_kafelek(plansza, score);
					rysuj_plansze(plansza, score);
					al_flip_display();
					status = 0;
					licznik_liter = 0;
					nick = al_ustr_new("");
					// I zmieniam okienko na granie
					okienko = 0;
				}
			}
		}
		else if (okienko == 2)
		{
			if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				// wcisniecie dalej
				if(tab_przyc[2].czy_wcisniety(ev.mouse.button,ev.mouse.x,ev.mouse.y)){
					status = 1;
					rysuj_plansze(plansza, score);
					tab_przyc[2].zmien_stan(1);
					rysuj_koniec(plansza, false, score);
					al_flip_display();
					tab_przyc[2].zmien_stan(0);
				}
			}
			else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
				if(status == 1){
					score = 0;
					rysuj_plansze(plansza, score);
					rysuj_koniec(plansza, false, score);
					al_flip_display();
					al_rest(0.1);
					plansza = przygotuj_plansze_start();
					plansza = dodaj_kafelek(plansza, score);
					rysuj_plansze(plansza, score);
					al_flip_display();
					status = 0;
					okienko = 0;
				}
			}
		}
	}
	return 0;
}