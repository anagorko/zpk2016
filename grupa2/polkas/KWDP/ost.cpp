
#include <iostream>

using namespace std;

main() {

int opt[4] = {6,2,4,8};
int n;
cin >> n;

if(n==0){cout << 1;} else {cout << opt[(n % 4)];}

}
