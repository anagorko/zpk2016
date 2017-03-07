#include <iostream>

using namespace std;

int main(){
unsigned int n;
cin >> n;
bool prime = true;
for (unsigned int i = 2; i < n / 2; i++){
if (n%i == 0) {
prime = false;
cout << "zlozona";
}
}
if (prime) cout << "pierwsza";
}
