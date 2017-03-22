
#include<stdlib.h>
#include <iostream>
#include <string>

using namespace std;

class Stack
{
    struct Element
    {
        Element* prev;
        float value;
    };

    Element* t;
public:
    Stack();    // inicjalizuje pusty stos
    ~Stack();   // kasuje wszystkie elementy

    float pop();   // kasuje gorny element stosu
    float top();  // zwraca wartosc na gorze stosu

    bool empty();     // true = stos jest pusty
    void push(float); // umieszcza wartosc na wierzcholku stosu
};

Stack::Stack(){
t=NULL;
}

 Stack::~Stack(){};

void Stack::push(float val){

Element * temp = new Element;

temp -> value = val;

if(t!=NULL) temp -> prev = t ;

t = temp;

}

float Stack::pop(){

if(t==NULL){ return 0 ;}
else{

Element * te = t -> prev;
float v = t->value;
delete t;
t = te;
return v;

}
}

float Stack::top(){

return t -> value;

}

bool Stack::empty(){
    return(t==NULL);
    }

int main()
{
    Stack s;

    while (!cin.eof()) {
        string token;
        cin >> token;

        if (token == "+") {
            float l, r;

            r = s.top(); s.pop();
            l = s.top(); s.pop();

            s.push(l + r);
        } else if (token == "*") {
            float l, r;

            r = s.top(); s.pop();
            l = s.top(); s.pop();

            s.push(l * r);
        } else if (token == "=") {
            cout << s.top() << endl;
        } else {
            s.push(atof(token.c_str()));
        }
    }
}
