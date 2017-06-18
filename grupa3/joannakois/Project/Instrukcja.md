# 2048

Joanna Kois

## Opis gry

Gra jest klonem napisanym w języku C++ oryginalnej gry 2048 stworzonej w językach JavaScript oraz CSS przez włoskiego programistę Gabrielego Cirulli. 
W oryginalną grę można zagrać [tutaj](https://gabrielecirulli.github.io/2048/). 

## Zasady gry

Krótko o zasadach gry jak i również ulubionych taktykach na zwycięstwo twórcy gry 2048 można przeczytać [tutaj](https://artykuly.softonic.pl/jak-wygrac-w-2048-tworca-gry-ujawnia-swoj-sekret). 

## Instalacja gry

W celu uruchomienia gry należy w dowolnym IDE dołączyć do projektu z zainstalowaną biblioteką do tworzenia gier 2D _Allegro_ plik _2048.cpp_. 
Należy również umieścić folder _media_ ze wszystkimi plikami graficznymi (_PNG_), dźwiękowymi (_WAV_) oraz plikami czcionek (_TTF_) w tej samej lokalizacji na dysku co stworzony projekt.
Po wykonaniu powyższych dwóch kroków kod należy skompilować i uruchomić. 

## Pliki wykorzystane w grze

Poniżej znajduje się spis potrzebnych do uruchomienia gry plików graficznych, dźwiękowych oraz plików czcionek.

* Pliki graficzne:
  * pusty.png, 2.png, 4.png, 8.png, 16.png, ... , 2048.png
  * Nowa_gra_0.png, Nowa_gra_1.png Cofnij_0.png, Cofnij_1.png, Wynik.png, Najlepszy_wynik.png, Przegrana.png, Wygrana.png
  * kursor.png
* Pliki dźwiękowe:
  * boo.wav, click_x.wav, fanfare3.wav
* Pliki czcionek:
  * Franklin Gothic Medium Regular.ttf, orange juice 2.0.ttf

## Opis funkcjonalności gry

Gra posiada jednookienkowy interfejs, który przypomina swoim wyglądem oryginalną postać gry. Ekran gry znajduje się poniżej.

<p align="center">
  <img src="http://imageshack.com/a/img923/3811/I9zi3p.png" alt="Screenshot"/>
</p>

Oprócz standardowych funkcjonalności zaczerpniętych z interfejsu oryginalenej gry (tj. *NOWA GRA*, okienko z punktacją, okienko z najlepszym wynikiem),
gra umożliwia także cofnięcie ruchu (przycisk *COFNIJ*). Wyjść z gry można za pomocą znaku *X* w prawym górnym rogu ekranu. 

- - - -

## Źródła 

**Gra napisana w oparciu o:**
* grę *dynablaster.cpp*;
* [konsolową wersję gry 2048](https://chandruscm.wordpress.com/2014/10/25/2048-in-c-c/);
* grę *GraphWar* - projekt z 2015 roku;
* [tutoriale Allegro](https://wiki.allegro.cc/index.php?title=Main_Page). 

**Źródła plików:**
* pliki graficzne (oprócz pliku _kursor.png_) - projekt własny;
 * plik _kursor.png_ pochodzi ze strony: http://www.freeiconspng.com/search.html?q=cursor&tip=png;
* pliki dźwiękowe pochodzą ze strony: http://www.wavsource.com;
* pliki czcionek pochodzą ze strony: http://www.1001freefonts.com.
