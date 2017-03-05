#include <iostream>
using namespace std;

int main()
{
    int b=0,a;
    cin>> a;
    if(a<1) {cout << "zly format"; return 0;}
    for(int i=1;i<=a;i++)
    {
        if(a%i==0) {b++;}
    }
    if(b>2)
        {cout << "Liczba zlozona";} else cout << "Liczba pierwsza";
    return 0;
}
