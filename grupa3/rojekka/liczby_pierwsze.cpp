#include <iostream>

using namespace std;

bool czy_pierwsza(int n){
    int l = 0;
    bool c = false;
    if(n == 1){
        c = true;
    }
    else if(n > 1){
        for(int i = 2; i <= n; i++){
            if(n%i==0){
                l++;
            }
        }
        if(l < 2){
            c = true;
        }
        else {
                c = false;
            }
    }
return c;
};

int main()
{
    int n;
    cin >> n;
    if(czy_pierwsza(n) == true){
        cout << "pierwsza";
    }
    else {
        cout << "zlozona";
    }

    return 0;
}
