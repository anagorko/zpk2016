#include <iostream>
using namespace std;

int main(){
int a,b,c;
cout << "Podaj pierwsza liczbe: ";
cin >> a;
cout << "Podaj druga liczbe: ";
cin >> b;
while (b!=0){
c = a % b;
a = b;
b = c;
}
cout << "Najwiekszy wspolny dzielnik: " << a << endl;
}
