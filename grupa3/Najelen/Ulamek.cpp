#include <iostream>

using namespace std;

class Ulamek {

  int l, m; // licznik , mianownik

public:

  Ulamek ();

  Ulamek(int _l, int _m);

  Ulamek& operator+=(const Ulamek &w);

  Ulamek operator+(const Ulamek &w);

  friend ostream& operator<<(ostream& os, const Ulamek &w);


};

Ulamek :: Ulamek (){

    l = 1;
    m = 1;

}

Ulamek::Ulamek(int _l, int _m){

    l = _l;
    m = _m;

}

Ulamek& Ulamek::operator+=(const Ulamek &w){

    l = l*w.m + w.l*m;
    m *= w.m;

    // skracanie

    int mn, wk;

    if (l>=m) {mn = m; wk = l;}
    else {mn = l; wk = m;}

    int d;

    while (mn != 0){

        d = wk%mn;
        wk = mn;
        mn = d;

    }

    l = l/wk;
    m = m/wk;

    return *this;

}

Ulamek Ulamek::operator+(const Ulamek &w){

    Ulamek tmp = *this;
    tmp += w;

    return tmp;

}

ostream& operator <<(ostream& os, const Ulamek &w){

    os << w.l << " / " << w.m;

    return os;

}
/*
int nwd(int l, int m){

    int mn, wk;

    if (l>=m) {mn = m; wk = l;}
    else {mn = l; wk = m;}

    int d;

    while (mn != 0){

        d = wk%mn;
        wk = mn;
        mn = d;

    }

    return wk;

}
*/
int main()
{

   Ulamek a(5,5), b(1,3), c(1,6); cout << (a + b + c) << endl;

}
