#include <iostream>

using namespace std;

// ulamek postaci xxxxxxxxxx.x (jedno miejsce po przecinku)

class Ulamek {
  int l, m; // licznik , mianownik
public:
 Ulamek(){
        l=0;
        m=0;
    };

Ulamek(int _l, int _m)
    {
        l=_l;
        m=_m;
    };

  //Ulamek& operator+=(const Ulamek &b);
  //Ulamek operator+(const Ulamek &b);
  friend ostream& operator<<(ostream& os, const Ulamek &w);

~Ulamek() {};


 Ulamek& operator+=(const Ulamek &b) {
    l = l*b.m+b.l*m;
    m = m*b.m;
     return *this;
  };

Ulamek operator+(const Ulamek &b) {
    Ulamek tmp = *this;
    tmp += b;
    return tmp;
  };
};


ostream& operator<<(ostream &os, const Ulamek &w) {
  os << w.l << "/" << w.m;
  return os;
}


int main()
{
 Ulamek a(3,8), b(1,8); cout << (a + b) << endl;
}
