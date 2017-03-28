#include<stdlib.h>
#include<iostream>
#include<string>


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
	Stack(){
		t = nullptr;
	}    // inicjalizuje pusty stos
	~Stack(){
		while (!this->empty()){
			this->pop();
		}
	}// kasuje wszystkie elementy

	void pop(){ 
		Element* temp = t; 
		t = temp->prev;
		delete temp;
	}   // kasuje gorny element stosu
	float top(){ 
		return(t->value);
	}  // zwraca wartosc na gorze stosu

	bool empty(){
		return(t==nullptr);
	}     // true = stos jest pusty
	void push(float val){ 
		Element* e = new Element;
		e->value = val;
		e->prev = t;
		t = e;
	} // umieszcza wartosc na wierzcholku stosu
};

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
		}
		else if (token == "*") {
			float l, r;

			r = s.top(); s.pop();
			l = s.top(); s.pop();

			s.push(l * r);
		}
		else if (token == "=") {
			cout << s.top() << endl;
		}
		else {
			s.push(atof(token.c_str()));
		}
	}
}
