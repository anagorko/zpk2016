#include <iostream>

using namespace std;

class Stack
{
    class Element
    {
        public:
            Element* prev;
            float value;
    };

    Element* t;

public:

    // inicjalizuje pusty stos
    Stack() {
        t -> prev = NULL;
    };

    // kasuje wszystkie elementy
    ~Stack();

    // kasuje gorny element stosu
    void pop(){
        if(t == NULL) {return; }
        t = t -> prev;
    };
    
    // zwraca wartosc na gorze stosu
    float top() {
        if (t != NULL) {return t -> value; }
    };  

    // true = stos jest pusty
    bool empty(){
        return t == NULL;
    };

    // umieszcza wartosc na wierzcholku stosu
    void push(float a){
        Element* x = new Element();
        x -> prev =t;
        t=x;
        t -> value = a;
    };
};

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
