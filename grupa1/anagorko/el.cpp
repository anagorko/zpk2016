#include <iostream>
using namespace std;

class EL {    
    int w;    
    EL *n;

public:    
    EL(int _w) {        
      w = _w;        
      n = NULL;    
    }    
    
    EL() : EL(0) { }
    
    void ustawNastepny(EL* _n) {        
      n = _n;    
    }    
    
    void wypisz() {        
      cout << w;    
    }    
    
    void wypiszWszystko() {
      wypisz();
      if (n == NULL) return;
      cout << " ";
      n -> wypiszWszystko();
    }
    
    EL* zwrocNastepny() {
      return n;    
    }
  };
  
  int main()
  { 
    EL* pierwszy = new EL(1);  
    EL* drugi = new EL(2);
    pierwszy -> ustawNastepny(drugi);
    drugi -> ustawNastepny(new EL(3)); 
    drugi -> zwrocNastepny() -> ustawNastepny(new EL(4));   
    EL* x = new EL(100); 
    x -> ustawNastepny(drugi -> zwrocNastepny()); 
    drugi -> ustawNastepny(x); 
    pierwszy -> wypiszWszystko(); 
    return 0;
  }
