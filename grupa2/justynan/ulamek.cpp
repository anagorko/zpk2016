#include <iostream>

using namespace std;

class Ulamek {
  int l, m; // licznik , mianownik

public:
  Ulamek ();
  Ulamek(int _l, int _m)
  {
        l = _l;
        m = _m;
  }
  Ulamek& operator+=(const Ulamek &w){
        l *= w.m;
        l += w.l * m;
        m *= m;
    return *this;
  }
  Ulamek operator+(const Ulamek &w){
        Ulamek tmp = *this;
        tmp += w;
        return tmp;
  }


  friend ostream& operator<<(ostream& os, const Ulamek &w);
};

ostream& operator<<(ostream& os, const Ulamek &w){
    os << w.l << "/" << w.m;
    return os;
}

int main()
{
  Ulamek a(3,8), w(1,8);
  cout << "Suma " << a << "+" << w << "=" << (a+w);
  return 0;
}
