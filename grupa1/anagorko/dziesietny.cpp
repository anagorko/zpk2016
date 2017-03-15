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
  cout << "Podaj dwa ulamki dziesietne: " << endl; 
  cout << "a="; cin >> a;
  cout << "b="; cin >> b;  
  cout << "Suma " << a << "+" << b << "=" << (a+b);  
  return 0;
}
