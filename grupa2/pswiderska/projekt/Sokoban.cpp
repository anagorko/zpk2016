#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <memory>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>

using namespace std;
//Część przygotowana przez Karolinę Sulbińską
//sciezki do gry oraz grafiki
const string project_path = "C:/Users/Patrycja/Desktop/Sokoban/";
const string lev_path = project_path + "levels";
const string menu_path = project_path + "graphics/menu.png";
const string next_lev = project_path + "graphics/next_level.png";
const string last_lev = project_path + "graphics/last_level.png";

//rozmiary planszy oraz czestosc wyswietlania
const int k_sz = 56;	// szerokość (piksele) kafelka
const int k_wy = 56;	// wysokość (piksele) kafelka
const float FPS = 60;       // Frames Per Second

// nowy typ danych
typedef vector<vector<char>> CharMatrix;

// tablica klawiszy
bool key[ALLEGRO_KEY_MAX];  // wciśnięte klawisze

// Kafelki - przechwouja informacje o typach kafelkow
enum typ_kafelka
{
	wall = 0,
	box, box_on_goal_square, goal_square, floor_square, player, player_on_goal_square,
	liczba_kafelkow
};

const string plik_z_kafelkiem[liczba_kafelkow] =
{
	"graphics/wall.png", // sciana
	"graphics/box.png", // pudełko
	"graphics/box_on_goal_square.png", // pudełko na właściwym miejscu
	"graphics/goal_square.png", // docelowe miejsce pudełka
	"graphics/floor.png", // podłoga
	"graphics/player.png", // gracz
	"graphics/player_on_goal_square.png", // gracz na docelowym miejscu pudełka
};

// BITMAPY

ALLEGRO_BITMAP* bitmapa[liczba_kafelkow];

bool przygotuj_bitmapy()
{
	for (int i = 0; i < liczba_kafelkow; i++) {
		bitmapa[i] = al_load_bitmap((project_path + plik_z_kafelkiem[i]).c_str());

		if (!bitmapa[i]) {
			cerr << "Błąd podczas wczytywania bitmapy " << plik_z_kafelkiem[i] << "." << endl;
			return false;
		}
	}

	return true;
}

// FUNKCJE POMOCNICZE DO OBSŁUGI PLANSZY

//Wczytanie planszy
shared_ptr<CharMatrix> read_level(string path) {
	int nrow = 0, ncol = 0;
	string numstr;
	ifstream f(path);
	if (f.is_open()) {
		getline(f, numstr, ',');
		istringstream str(numstr);
		str >> nrow;
		getline(f, numstr);
		istringstream str2(numstr);
		str2 >> ncol;
		getline(f, numstr); //skip blank line;
	}
	CharMatrix m(nrow, vector<char>(ncol));

	if (f.is_open()) {
		char a;
		for (int i = 0; i < nrow; i++)
			for (int j = 0; j <= ncol; j++) {
				f >> noskipws >> a;
				if (a != '\n') m[i][j] = a;
			}
	}
	f.close();
	return(make_shared<CharMatrix>(m));
}

//Zapisanie planszy
void save_level(shared_ptr<CharMatrix> m, string path) {
	int nrow, ncol;
	nrow = m->size();

	ncol = m->at(0).size();
	ofstream f(path);
	f << nrow << ',' << ncol << endl << endl;
	for (int i = 0; i < nrow; i++) {
		for (int j = 0; j < ncol; j++)
			f << (*m)[i][j];
		f << endl;
	}
	f.close();
}

// KLASY

// kierunek poruszania sie na planszy
struct kierunek{
	int dx, dy;
	// operatory skalowania
	kierunek operator*(int i){
		kierunek k;
		k.dx = dx*i;
		k.dy = dy*i;
		return k;
	}

	friend kierunek operator*(int i, kierunek k){
		kierunek kk;
		kk = k*i;
		return kk;
	}
};

// definicja klasy pozycja dla skrócenia zapisu
class pozycja
{
public:
	int x, y;
	pozycja operator+(kierunek k) {
		pozycja pnew;
		pnew.x = x + k.dx;
		pnew.y = y + k.dy;
		return pnew;
	}
};

//klasa gracz
class gracz{

public:
	pozycja p;
	gracz(CharMatrix m){
		p.x = -1; p.y = -1;
		for (int i = 0; i < m.size(); i++)
			for (int j = 0; j < m[i].size(); j++)
				if (m[i][j] == '@'){
					p.x = i; p.y = j;
				}
		if (p.x == -1) cerr << "Nie ma gracza na planszy!" << endl;
	}
	 void move(int dx, int dy){
		p.x += dx;
		p.y += dy;
		return;
	}
	 void move(kierunek k){
		this->move(k.dx, k.dy);
		return;
	}

};

//klasa obslugujaca zachowanie planszy
class plansza{

	int wy, sz;

public:
	CharMatrix m;
	plansza(string path){
		m = *read_level(path);
		wy = m.size();
		sz = m[0].size();
	}
	
				
		// Logika poruszania się
	bool move(gracz& g, kierunek k){
		pozycja pnew = g.p + k;
		pozycja pnew2 = g.p + 2 * k;
		char znew = m[pnew.x][pnew.y];

		if (znew == '#') return false;
		if (znew == '$' || znew == '*'){
			char znew2 = m[pnew2.x][pnew2.y];
			if (znew2 == '#' || znew2 == '$' || znew2 == '*') return false;
			push(pnew, pnew2);
			push(g.p, pnew);
			g.move(k);
			return true;
		}
		push(g.p, pnew);
		g.move(k);
		return true;
	}
	void push(pozycja p, pozycja pnew){
		char z = m[p.x][p.y];
		char znew = m[pnew.x][pnew.y];
		// Co pozostaje po przesunieciu?;
		if (z == '@' || z == '$') {
			m[p.x][p.y] = ' ';
			if (znew == ' ') m[pnew.x][pnew.y] = z;
			if (znew == '.' && z == '@') m[pnew.x][pnew.y] = '+';
			if (znew == '.' && z == '$') m[pnew.x][pnew.y] = '*';
		}
		if (z == '+' || z == '*') {
			m[p.x][p.y] = '.';
			if (znew == '.') m[pnew.x][pnew.y] = z;
			if (znew == ' ' && z == '+') m[pnew.x][pnew.y] = '@';
			if (znew == ' ' && z == '*') m[pnew.x][pnew.y] = '$';
		}
	}
//Część przygotowana przez Patrycję Świderską
	void rysuj_plansze();
	bool sprawdzanie()
	{
		for (int i = 0; i < wy; i++)
			for (int j = 0; j < sz; j++)
				if (m[i][j] == '$') { return false; }
		return true;
	};
};

void plansza::rysuj_plansze()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));

	for (int i = 0; i < wy; i++) {
		for (int j = 0; j < sz; j++) {
			// sciana
			if (m[i][j] == '#') al_draw_bitmap_region(bitmapa[wall], 0.0, 0.0, k_sz, k_wy, i*k_sz, j*k_sz, 0);
			// podloga
			if (m[i][j] == ' ') al_draw_bitmap_region(bitmapa[floor_square], 0.0, 0.0, k_sz, k_wy, i*k_sz, j*k_sz, 0);
			if (m[i][j] == '.') al_draw_bitmap_region(bitmapa[goal_square], 0.0, 0.0, k_sz, k_wy, i*k_sz, j*k_sz, 0);
			// pudla
			if (m[i][j] == '$') al_draw_bitmap_region(bitmapa[box], 0.0, 0.0, k_sz, k_wy, i*k_sz, j*k_sz, 0);
			if (m[i][j] == '*') al_draw_bitmap_region(bitmapa[box_on_goal_square], 0.0, 0.0, k_sz, k_wy, i*k_sz, j*k_sz, 0);
			//gracz
			if (m[i][j] == '@') al_draw_bitmap_region(bitmapa[player], 0.0, 0.0, k_sz, k_wy, i*k_sz, j*k_sz, 0);
			if (m[i][j] == '+') al_draw_bitmap_region(bitmapa[player_on_goal_square], 0.0, 0.0, k_sz, k_wy, i*k_sz, j*k_sz, 0);
		}
	}
};

//definicja kierunkow gracza
enum direction{
	gora = 0,
	prawo = 1, dol = 2, lewo = 3, liczba_kierunkow = 4
};

const kierunek ruch[liczba_kierunkow] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };

//inicjalizacja biblioteki allegro
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;


int init(int screen_w, int screen_h)
{

	if (!al_init()) {
		cerr << "Błąd podczas inicjalizacji allegro." << endl;
		return -1;
	}

	if (!al_init_primitives_addon()) {
		cerr << "Błąd podczas inicjalizacji dodatku 'primitives'." << endl;
		return -1;
	}

	if (!al_init_image_addon()) {
		cerr << "Błąd podczas inicjalizacji dodatku 'image'." << endl;
		return -1;
	}

	if (!al_install_keyboard()) {
		cerr << "Błąd podczas inicjalizacji klawiatury." << endl;
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		cerr << "Błąd podczas inicjalizacji zegara." << endl;
		return -1;
	}

	display = al_create_display(screen_w, screen_h);
	if (!display) {
		cerr << "Błąd podczas inicjalizacji ekranu." << endl;
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		cerr << "Błąd podczas inicjalizacji kolejki zdarzeń." << endl;
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();
	al_start_timer(timer);

	return 0;
}

int main(){
	//GŁÓWNE MENU - WYBÓR POZIOMU
	int level = 0;
start:
// wczytanie grafiki do menu
	ALLEGRO_BITMAP* menu;

	if (init(500, 500) != 0) {
		cerr << "Inicjalizacja nie powiodła się." << endl;
		return -1;
	}
	menu = al_load_bitmap(menu_path.c_str());
	al_draw_bitmap(menu, 0.0, 0.0, 0);
	al_flip_display();

//wybor poziomu
	while (true)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			if (ev.keyboard.keycode == ALLEGRO_KEY_1) {
				level = 1;
				al_destroy_display(display);
				break;
			};
			if (ev.keyboard.keycode == ALLEGRO_KEY_2) {
				level = 2;
				al_destroy_display(display);
				break;
			};
			if (ev.keyboard.keycode == ALLEGRO_KEY_3) {
				level = 3;
				al_destroy_display(display);
				break;
			};
			if (ev.keyboard.keycode == ALLEGRO_KEY_4) {
				level = 4;
				al_destroy_display(display);
				break;
			};
			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				al_destroy_display(display);
				return -1;
				break;
			}
		}
	};

	if (level == 0) { return -1; };
//gra
	for (int i = level; i <= 4; i++)
	{
	nxt_lev:
    //wczytanie pliku z wybranym poziomem
		ostringstream ss;
		ss << i;
		string lv_str = ss.str();
		int nrow, ncol;
		string lev = lev_path + "/" + "lev" + lv_str + ".txt";
		string save_lev = lev_path + "/" + "lev" + lv_str + "_save.txt";
		shared_ptr<CharMatrix> m = read_level(lev);
		nrow = m->size();
		ncol = m->at(0).size();

		if (init(nrow*k_wy, ncol*k_sz) != 0) {
			cerr << "Inicjalizacja nie powiodła się." << endl;
			return -1;
		}
		//wczytanie bitmap
		if (!przygotuj_bitmapy()) {
			return -1;
		};

		plansza p(lev);
		gracz g(p.m);
		p.rysuj_plansze();

		al_flip_display();

		while (true)
		{
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);

			if (ev.type == ALLEGRO_EVENT_TIMER) {
				p.rysuj_plansze();
				al_flip_display();
			}
			//poruszanie sie gracza po planszy za pomoca klawiszy
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
				if (ev.keyboard.keycode == ALLEGRO_KEY_UP){
					p.move(g, ruch[gora]);
				}
				else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
					p.move(g, ruch[dol]);
				}
				else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
					p.move(g, ruch[prawo]);
				}
				else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
					p.move(g, ruch[lewo]);
				}
			}
			//po przejściu poziomu, wyswietla sie pytanie, czy gracz chce przejsc do kolejnego poziomu
			if (p.sprawdzanie()) {
				if (i != 4){
					al_destroy_display(display);
					ALLEGRO_BITMAP* next_level;
					if (init(500, 500) != 0) {
						cerr << "Inicjalizacja nie powiodła się." << endl;
						return -1;
					}
					next_level = al_load_bitmap(next_lev.c_str());
					al_draw_bitmap(next_level, 0.0, 0.0, 0);
					al_flip_display();
                    //decyzja gracza
					while (true)
					{
						ALLEGRO_EVENT ev;
						al_wait_for_event(event_queue, &ev);
                        //mozliwe jest rowniez zapisanie wygranego poziomu
						if (ev.type == ALLEGRO_EVENT_KEY_UP) {
							if (ev.keyboard.keycode == ALLEGRO_KEY_S) {
								save_level(m, save_lev);
							};
                            //gracz decyduje sie przejsc do kolejnego poziomu
							if (ev.keyboard.keycode == ALLEGRO_KEY_Y) {
								al_destroy_display(display);
								i++;
								goto nxt_lev;
							};
							//gracz nie chce przejsc do kolejnego poziomu
							if (ev.keyboard.keycode == ALLEGRO_KEY_N || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
								al_destroy_display(display);
								return -1;
								break;

							}
						}
					}
				}
                //przypadek wygrania odtatniego poziomu
				if (i == 4){
					al_destroy_display(display);
					ALLEGRO_BITMAP* last_level;
					if (init(500, 500) != 0) {
						cerr << "Inicjalizacja nie powiodła się." << endl;
						return -1;
					}
					last_level = al_load_bitmap(last_lev.c_str());
					al_draw_bitmap(last_level, 0.0, 0.0, 0);
					al_flip_display();

					while (true)
					{
						ALLEGRO_EVENT ev;
						al_wait_for_event(event_queue, &ev);
						//możliwosc zapisania wygranego poziomu
						if (ev.type == ALLEGRO_EVENT_KEY_UP) {
							if (ev.keyboard.keycode == ALLEGRO_KEY_S) {
								save_level(m, save_lev);
							};
                            //wyjscie z gry
							if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
								al_destroy_display(display);
								return -1;
								break;
							}
						}
					}
				}
			};
            //powrot do menu podczas gry
			if (ev.type == ALLEGRO_EVENT_KEY_UP) {
				if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					al_destroy_display(display);
					goto start;

				}
			}
		}
	}
	return 0;
}
