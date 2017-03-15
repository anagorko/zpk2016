#include <iostream>

using namespace std;

class Ulamek {
  int l, m; // licznik , mianownik
public:
  Ulamek (){
  l=0;
  m=0;
  };

Ulamek(int _l, int _m)
    {
        l=_l;
        m=_m;
    };

Ulamek& operator+=(const Ulamek &w)
    {
        l=l*w.m+m*w.l;
        m=m*w.m;

        return *this;
    }

Ulamek operator+(const Ulamek &w)
    {
        Ulamek tmp=*this;
        tmp+=w;
        return tmp;

    }

friend ostream& operator<<(ostream& os, const Ulamek &w);
};

ostream& operator<<(ostream &os,const Ulamek &w)
    {
    os<<w.l << "/" << w.m;
    return os;
    }

int main()
{
   Ulamek a(4,8), b(1,8); cout << (a + b) << endl;
}

