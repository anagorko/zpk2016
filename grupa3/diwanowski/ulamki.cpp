#include <iostream>

using namespace std;

class Ulamek {
  int l, m; // licznik , mianownik
public:
  Ulamek () {};
  Ulamek(int _l, int _m){
    l = _l;
    m = _m;
  };

  Ulamek& operator+=(const Ulamek &w){
    l = w.l * m + l * w.m;
    m = w.m * m;
    return *this;
  };
  Ulamek operator+(const Ulamek &w){
    Ulamek tmp = *this;
    tmp += w;
    return tmp;
  };
  friend ostream& operator<<(ostream& os, const Ulamek &w);
};

ostream& operator<<(ostream& os, const Ulamek &w){
    os << w.l << "/" << w.m;
    return os;
}

int main()
{
Ulamek a(1,5), b(2,5); cout << (a + b) << endl;
}
