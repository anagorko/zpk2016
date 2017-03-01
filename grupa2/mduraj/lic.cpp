#include <iostream>

using namespace std;

int liczba;
string wynik;

int main()
{

    cin >> liczba;

    wynik = "pierwsza";

    for (int i=2; i<liczba; i++)

    {

        if (liczba % i == 0)
        {
            wynik = "zlozona";
        }
    }

    cout << wynik;

    }
