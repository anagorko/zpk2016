#include <iostream>
using namespace std;

//  Funkcja zwracajaca true, gdy liczba jest pierwsza
//  i false, gdy jest zlozona
bool pierwsza(int liczba) {
    for (int i = 2; i < liczba; i++)
        if (liczba % i == 0)
            return false;
    return true;
}
//  Program wlasciwy
int main() {
    int wybrana;
    cin >> wybrana;
        if (pierwsza(wybrana) == true)
             cout << "pierwsza" << endl;
        else
            cout << "zlozona" << endl;
}
