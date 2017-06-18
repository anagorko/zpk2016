#include<iostream>
using namespace std;
#include <math.h>
#include <stack>
#include <queue>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

/* 
  Projekt zaliczeniowy bazuje na odwrotnej notacji polskiej według alogrytmu: 
  https://pl.wikipedia.org/wiki/Odwrotna_notacja_polska
  oraz kodu z zajęć: 
  https://github.com/anagorko/zpk2016/blob/master/allegro/plot.cpp 
*/

const double XMIN=-10; //zakres osi
const double XMAX=10; //zakres osi
const int W=800; //szerokość okienka
const int H=600; //wysokość okienka

double sup, inf; //definicja supremum i infimum, kres górny i dolny
vector <double> v; 

/*
  Funkcje int zero_x oraz int zero_y pochodzą z kodu z zajęć: 
  https://github.com/anagorko/zpk2016/blob/master/allegro/plot.cpp
*/
int zero_x()
{
    return W * (0 - XMIN) / (XMAX - XMIN);
}

int zero_y()
{
    return H * (0 - inf) / (sup - inf);
}

bool czyjestdzialaniem(string x)
/* 
  Zadaniem funkcji jest sprawdzenie czy symbol wpisany z klawiatury 
  jest operatorem/działaniem typu +,-, *,/, ^, sin, cos, tg czy ctg. 
*/
{
	if(x=="+" || x=="-" || x=="*" || x=="/" || x=="^")
	{
		return true;
	}
	if(x=="sin"||x=="cos"||x=="tg"||x=="ctg")
	{
		return true;
	}
	return false;
} 

double obliczdzialanie (string x, double a, double b=0.0)
{	double wynik;

	if(x=="+")
	{
		wynik=b+a;
	}
	if(x=="-")
	{
		wynik=b-a;
	}
	if(x=="*")
	{
		wynik=b*a;
	}
	if(x=="/")
	{
		wynik=b/a;
	}
	if(x=="^")
	{
		wynik=pow(b,a);
	}
	if(x=="sin")
	{
		wynik=sin(a);
	}
	if(x=="cos")
	{
		wynik=cos(a);
	}
	if(x=="tg")
	{
		wynik = tan(a);
	}
	if(x=="ctg")
	{
		wynik = 1.0/tan(a);
	}
	return wynik;
}

int ilelementow(string x)
/*
  ta funkcja mówi ilu argumentowym dzialaniem jest x (zakładamy, że x jest dzialaniem). 
  Funkcje trygonometryczne są funkcjami jednoelementowymi zatem dla nich funkcja zwróci 1, dla innych 2.
*/
{
	if(x=="sin"||x=="cos"||x=="tg"||x=="ctg")
	{
		return 1;
	}
	return 2; 
} 

double obliczfunkcje (queue <string> q, double wartoscx)
/*
  Argumentem jest kolejka zmiennych typu string q, czyli definicja funkcji (zapisana w ONP) do narysowania wykresu 
  oraz wartoscx czyli wartość jaką należy podstawić za x podczas obliczania funkcji.
*/
{
	stack <double> stos; 
	while(!q.empty()) 
	{
		string poczatekkolejki; 
		poczatekkolejki = q.front();
		q.pop(); 
		/* 
		  Empty, pop, front i top są funkcjami z biblioteki <queue>.
		  Empty sprawdza czy kolejka jest pusta.
		  Pop usuwa element z początku kolejki.
		  Front zwraca pierwszy element w kolejce.
		*/
		if(czyjestdzialaniem(poczatekkolejki)) 
			/* 
			  Wykonuje się jeśli symbol jest operatorem/działaniem. 
			*/
		{
			double a=0, b=0; 
			/*
			  ustawiamy wartości początkowe na 0 na wszelki wypadek, żeby miec pewność, że jakieś śmieci z pamięci nie wpłyną nam na wynik.
			*/
			
			if(ilelementow(poczatekkolejki)==2) 
			/*
			  Ta funkcja if wykonuje się gdy mamy działanie dwuelementowe.
			  Alogyrtm na podstawie: 
			  https://pl.wikipedia.org/wiki/Odwrotna_notacja_polska
			*/
			{	
				a=stos.top(); //Zmiennej "a" przypisuję wartość z szczytu stosu 
				stos.pop(); //zdejmuję element z szczytu stosu
				b=stos.top(); ////Zmiennej "b" przypisuję wartość z szczytu stosu po zdjęciu elementu "a" z szczytu stosu
				stos.pop(); //zdejmuję element z szczytu stosu
			}
			else 
			/*
			  jeśli jest działaniem jednoelementowym
			*/
			{
				a=stos.top(); 
				stos.pop();
			}
			double wynik = obliczdzialanie(poczatekkolejki,a,b);

			stos.push(wynik);
		} 
		else if (poczatekkolejki=="x")
		{
			stos.push(wartoscx); 
			/*
			  wartoscx to jest wartość jaką podstawiamy za x podczas obliczania funkcji:
			  więcej w funkcji "obliczwykres"
			*/
		}
		else 
		/*
		  Wykona się gdy obiekt wpisany z klawiatury nie jest ani działaniem ani "x", tylko liczbą.
		*/
		{
			double y = stod(poczatekkolejki); 
			/*
			  stod - string to double, funkcja należąca do biblioteki <string>. 
			  Do zmiennej y przepisujemy wartość wpisaną z klawiatury zmieniając ją z typu string na typ double.
			*/
			stos.push(y);
		}

	}
	return stos.top(); 
}

void obliczwykres(queue <string> q) 
/*
  kod na podstawie kodu z zajęć: https://github.com/anagorko/zpk2016/blob/master/allegro/plot.cpp
*/
{	
	sup = -1000000;
    inf = 1000000;
	for (int i = 0; i < W; i++) 
	{
		double y = obliczfunkcje(q,XMIN + ((XMAX - XMIN) * i) / W);  
		/* 
		  Drugi argument funkcji został stworzony na podstawie kodu z zajęć: 
		  https://github.com/anagorko/zpk2016/blob/master/allegro/plot.cpp
		*/
		sup = max(sup,y);
		inf = min(inf,y);
		v.push_back(y); 
	}
}

double absd(double x)
/* 
  Funkcja napisana przez Pana podczas konsultacji projektów. 
*/
{
	return x >= 0 ? x : -x;
}

int main(int argc, char** argv)
{	
	string x;
	cin>>x; 
	queue <string> q;
	while(x!="end")
	{
		q.push(x);
		cin>>x;
	}
	/*
	  kod na podstawie kodu z zajęć: https://github.com/anagorko/zpk2016/blob/master/allegro/plot.cpp
	*/
	ALLEGRO_DISPLAY       *display = NULL; 
 
    al_init();
    al_init_primitives_addon();

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
 
    display = al_create_display(W, H);

	obliczwykres(q);

	al_clear_to_color(al_map_rgb(0,0,0));     
    al_draw_line(0, H-zero_y(), W, H-zero_y(), al_map_rgb(255,255,255), 2); 
    /*
	    kod na podstawie kodu z zajęć: https://github.com/anagorko/zpk2016/blob/master/allegro/plot.cpp
    */
    al_draw_line(zero_x(),0, zero_x(), H, al_map_rgb(255,255,255), 2);
    /*
	    kod na podstawie kodu z zajęć: https://github.com/anagorko/zpk2016/blob/master/allegro/plot.cpp
    */ 


    for (int i = 0; i < W; i++) {
    	cout<<v[i]<<endl; 
    	/*
          Wypisuje wartości, pomaga sprawdzić czy funkcja poprawnie liczy.
    	*/
        al_draw_pixel(i, H - (v[i] / (sup - inf) * H + zero_y()), al_map_rgb(255,255,255));
    } 
    /*
      kod na podstawie kodu z zajęć: https://github.com/anagorko/zpk2016/blob/master/allegro/plot.cpp
    */ 
    al_flip_display();
    
    al_rest(3);
    al_destroy_display(display);    

	return 0;
}
