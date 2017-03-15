
#include <iostream>

using namespace std;

void funkcja_wsk1(int *wsk,int rozmiar);
void funkcja_wsk2(int *wsk,int rozmiar);
void funkcja_tab(int tab[],int rozmiar);

main(){

int tab[4] = {22,33,44,55};

funkcja_wsk1(tab,4);
funkcja_wsk2(tab,4);
funkcja_tab(tab,4);

}

void funkcja_wsk1(int *wsk,int rozmiar){

cout << "Funkcja *()"<<endl;

for(int i =0; i<rozmiar;i++){ cout<< *(wsk++)<<endl;}

}

void funkcja_wsk2(int *wsk,int rozmiar){

cout << "Funkcja wsk[]"<<endl;

for(int i=0; i<rozmiar;i++){ cout<< wsk[i] <<endl;}

}

void funkcja_tab(int tab[],int rozmiar){

cout << "Funkcja tab[]"<<endl;

for(int i=0; i<rozmiar;i++){ cout<< tab[i]<<endl;}

}
