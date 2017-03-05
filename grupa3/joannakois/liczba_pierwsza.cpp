#include <iostream>
#include <string>
using namespace std;

string czy_pierwsza(int n) {
    for (int i = 2; i < n; i++)
        if (n % i == 0)
            return "zlozona";
    return "pierwsza";
}

int main() {
    int n;
    cin >> n;
    cout << czy_pierwsza(n) << endl;
}
