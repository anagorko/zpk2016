#include <iostream>

using namespace std;

int n;
int t = 0;

int main()
{
   cin >> n;
   for (int i = 2; i <= n/2; i++) {
    if (n%i == 0) {t++;}
   }
   if (t > 0) {cout << "zlozona";}
   else {cout << "pierwsza";}
}
