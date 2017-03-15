#include <iostream>
using namespace std;


class Ulamek
{
    int l; //licznik
    int m; //mianownik

public:

    Ulamek() {}
    Ulamek(int _l, int _m)
    {
        l = _l;
        m = _m;
    }

    Ulamek& operator+=(Ulamek &w){
        w.l = w.l * m;

        l = l * w.m + w.l;
        m = m * w.m;

        return *this;
    }

    Ulamek operator+(Ulamek &w){
        Ulamek tmp = *this;

        tmp += w;

        return tmp;
    }

    friend ostream& operator<<(ostream &o, Ulamek b);

};


//ulamki sa postaci 41,5

ostream& operator<<(ostream &o, Ulamek b)
{
    o << b.l << " / " << b.m;

    return o;
}

int main()
{
    Ulamek a(3,8), b(1,8);

    cout << (a + b) << endl;

    return 0;
}

