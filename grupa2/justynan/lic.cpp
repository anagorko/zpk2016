#include <iostream>
using namespace std;
 
int main() {
	int n, i = 2;
	cin >> n;
	if (n >= 2){

		while (n%i != 0){
			i++;
		}

		if ( i == n)
			cout << "pierwsza" << endl;		
		else
			cout << "zlozona" << endl;

	}
}
