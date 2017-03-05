#include <iostream>
using namespace std;

int czyPierwsza(int liczba)
{   int a = 1;
    for(int i = 2; i <= liczba-1; i = i++)
    {
        if (liczba%i == 0)
        {
            a = 0;
        }
    }
    return a;
}

int main()
{
    cout << "Liczba do sprawdzenia" << endl;

    int liczba;
    cin >> liczba;

    if(czyPierwsza(liczba)==1)
    {
        cout << "pierwsza" << endl;
    }
    else
    {
        cout << "zlozona" << endl;
    }
}
