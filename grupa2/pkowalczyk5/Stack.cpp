#include <iostream>
#include <stdlib.h>

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
    Stack()
    {
        t = NULL;
    } // inicjalizuje pusty stos

    ~Stack()
    {
    } // kasuje wszystkie elementy

    void pop()   // kasuje gorny element stosu
    {
        if (t == NULL){return;}

        t = t -> prev;
    }

    float top()  // zwraca wartosc na gorze stosu
    {
        if (t != NULL) {return t -> value; }
    }

    bool empty(){
        if(t == NULL) return true;
        else return false;
    }     // true = stos jest pusty

    void push(float x) // umieszcza wartosc na wierzcholku stosu
    {
        Element* w = new Element;
        w -> value = x;
        w -> prev = t;

        t = w;
    }
};

int main()
{
    Stack s;

    while (!cin.eof())
    {
        string token;
        cin >> token;



        if (token == "+")
        {
            float l, r;

            r = s.top(); s.pop();
            l = s.top(); s.pop();

            s.push(l + r);
        }

        else if (token == "*")
        {
            float l, r;

            r = s.top(); s.pop();
            l = s.top(); s.pop();

            s.push(l * r);
        }

        else if (token == "=")
        {
            cout << s.top() << endl;
            break; //wyjscie po wykonaniu dzialan, zakladam ze "=" konczy polecenie
        }

        else
        {
            s.push(atof(token.c_str()));
        }
   }

}
