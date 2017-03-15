#include <iostream>

using namespace std;

// ulamek postaci x / y

class Ulamek {
  int l; // licznik
  int m; // mianownik


public:
    Ulamek (){};
    Ulamek(int _l, int _m) {
    l= _l;
    m = _m;
    }

    friend istream& operator>>(istream& i, Ulamek& d);
    friend ostream& operator<<(ostream &o, Ulamek d);

    Ulamek& operator+=(const Ulamek &w){
    l=l*w.m+w.l*m;
    m=m*w.m;
    return *this;
    }
    Ulamek operator+(const Ulamek &w){
    Ulamek tmp = *this;
    tmp+=w;
    return tmp;
    }
};


istream& operator>>(istream& i, Ulamek& d) {
  i >> d.l;
  char tmp;
  i >> tmp;
  i >> d.m;
  return i;
}

ostream& operator<<(ostream &o, Ulamek d) {
  o << d.l << "/" << d.m;
  return o;
}

int main()
{
  Ulamek a(3,8), b(1,8); cout << (a + b) << endl;
  return 0;
}
