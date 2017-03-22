class Stack{ 
 
  struct Element    
  {        
    Element* prev;        
    float value;    
  };    
  
  Element* t;
  
public:    
  Stack() { t = NULL; }    // inicjalizuje pusty stos    
  ~Stack() {}   // kasuje wszystkie elementy    
  void pop(){t=t->prev;}   // kasuje gorny element stosu    
  float top() { return t->value; }  // zwraca wartosc na gorze stosu    
  bool empty(){return t==NULL;}     // true = stos jest pusty    
  void push(float a){        
    Element *wsk;        
    wsk = new Element;        
    wsk->prev = t;        
    wsk->value =a;        
    t=wsk;  }; // umieszcza wartosc na wierzcholku stosu};
    };
