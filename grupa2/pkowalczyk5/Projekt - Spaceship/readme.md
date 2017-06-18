# Spaceship
###### Autor: Paweł Kowalczyk
Projekt zaliczeniowy jest grą typu side-scroller, w której gracz kieruje niewielkim statkiem kosmicznym, a jego zadaniem jest unikanie pojawiających się na drodze przeszkód. 

### 1. Repozytorium
Ważne: przed uruchomieniem programu należy posiadać zainstalowaną bibliotekę Allegro w wersji 5.0.
W folderze znajdują się wszystkie pliki niezbędne do kompilacji projektu po wcześniejszej konfiguracji biblioteki Allegro dla niego. Alternatywnie można utworzyć nowy projekt, kopiując do niego zawartość pliku main.cpp oraz niezbędne grafiki i czcionki:
#### Czcionki
  * font-courbi.ttf
  * font-OptimusPrinceps.ttf
#### Pliki graficzne
  * spaceship-v3-4flames.png
  * propeller-bitmap.png

### 2. Instrukcja gry
Po uruchomieniu przez chwilę wyświetla się ekran startowy, a następnie zostaje pokazane główne menu. Nawigacja w programie odbywa się wyłącznie za pomocą klawiatury. W głównym menu możliwe jest rozpoczęcie nowej gry (klawisz 1), przejście do ustawień (klawisz 2), lub wyjście z programu (klawisz 3 lub Escape). Na ekranie ustawień za pomocą klawiszy z odpowiednimi numerami można wybrać poziom trudności lub powrócić do menu głównego. Po rozpoczęciu nowej gry wyświetlony zostaje statek gracza u dołu ekranu, a po chwili rozpoczyna się generowanie przeszkód. Sterowanie odbywa się za pomocą klawiszy strzałek. W dowolnej chwili można włączyć/wyłączyć pauzę (spacja) lub przerwać grę i wyjść do menu (Escape). Gra toczy się do momentu rozbicia statku o przeszkodę lub rezygnacji gracza.

### 3. Prawa autorskie
Oświadczam, że kod programu został przeze mnie napisany samodzielnie. Wykorzystałem w nim w niezbędnym zakresie gotowe biblioteki i dodatki:
  * math.h - ze standardowych zasobów języka C++
  * allegro.h, allegro_primitives.h, allegro_image.h, allegro_font.h, allegro_ttf.h - z podstawowej biblioteki Allegro 5.0

Wykorzystane czcionki:
  * font-courbi.ttf - standardowa czcionka Courier New z pogrubieniem i pochyleniem, pobrana z czcionek systemu Windows
  * font-OptimusPrinceps.ttf - czcionka pobrana z http://www.dafont.com/optimusprinceps.font, zgoda autora na prywatne wykorzystanie jego czcionek: http://manfred-klein.ina-mar.com/

Pliki graficzne (spaceship-v3-4flames.png, propeller-bitmap.png) zostały wykonane przez mnie przy użyciu darmowego oprogramowania GIMP oraz Inkscape (pliki źródłowe do wglądu na życzenie).
