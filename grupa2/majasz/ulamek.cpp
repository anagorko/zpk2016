#include <iostream>

using namespace std;

class Ulamek {
  int l, m; // licznik , mianownik
public:
  Ulamek (){}
  Ulamek(int _l, int _m)
  {
    l=_l;
    m=_m;
  }
  Ulamek& operator+=(const Ulamek &w)
  {
        l=l*w.m+w.l*m;
        m=m*w.m;

        return *this;
  }
  Ulamek operator+(const Ulamek &w)
  {
    Ulamek tmp = *this;
    tmp += w;
    return tmp;
  }

  friend istream& operator>>(istream& i, Ulamek& d);
  friend ostream& operator<<(ostream& os, const Ulamek &w);

};
istream& operator>>(istream& i, Ulamek& d) {
  i >> d.l;
  char tmp;
  i >> tmp;
  i >> d.m;
  return i;
}
ostream& operator<<(ostream& os, const Ulamek &w)
{
    return os << w.l <<"/" << w.m;
};
int main()
{
     Ulamek a, b;
  cout << "Podaj dwa ulamki dziesietne: " << endl;
  cout << "a="; cin >> a;
  cout << "b="; cin >> b;
  cout << "Suma " << a << "+" << b << "=" << (a+b);
return 0;
}
