#include <iostream>

using namespace std;

// ulamek postaci xxxxxxxxxx.x (jedno miejsce po przecinku)

class Dziesietny {
  int c; // czesc calkowita
  int u; // czesc ulamkowa

public:
  friend istream& operator>>(istream& i, Dziesietny& d);
  friend ostream& operator<<(ostream &o, Dziesietny d);
  Dziesietny& operator+=(Dziesietny b) {
    c += b.c;
    u += b.u;
    if (u >= 10) {
      u -= 10;
      c += 1;
    }
    return *this;
  }

  Dziesietny operator+(Dziesietny b) {
    Dziesietny tmp = *this;
    tmp += b;
    return tmp;
  }
};

class Ulamek {
  int l;
  int m; // licznik , mianownik

public:
  Ulamek ();
  Ulamek(int _l, int _m);
  friend istream& operator>>(istream& is, const Ulamek &w);
  friend ostream operator<<(ostream& os, const Ulamek &w);

  Ulamek& operator+=(Ulamek w){
  l=l*w.m+w.l*m;
  m=m*w.m;

  return *this;
    }

    Ulamek operator+(Ulamek w) {
    Ulamek tmp = *this;
    tmp += w;
    return tmp;
  }
};

istream& operator>>(istream& is, Ulamek w) {
  is >> w.l;
  char tmp;
  is >> tmp;
  is >> w.m;
  return is;
}


ostream& operator<<(ostream& os, Ulamek w) {
  os << w.l << "/" << w.m;
  return os;
}



istream& operator>>(istream& i, Dziesietny& d) {
  i >> d.c;
  char tmp;
  i >> tmp;
  i >> d.u;
  return i;
}

ostream& operator<<(ostream &o, Dziesietny d) {
  o << d.c << "." << d.u;
  return o;
}


int main()
{
  Dziesietny a, b;
  Ulamek w, x;
  cout << "Podaj dwa ulamki zwykle: " << endl;
  cin >> w;
cin >> x;
  cout << "Suma " << w << "+" << x << "=" << (w+x);
  return 0;
}
