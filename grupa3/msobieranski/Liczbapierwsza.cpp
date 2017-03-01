#include <iostream>

using namespace std;



int main()
{
    int n;
    cin>>n;

    for(int i=2; i<n; i++){

        if (n%i ==0){
            cout<<"ZLOZONA"<<endl;
            return 0;
        }
    }

    cout << "PIERWSZA"<<endl;
    return 0;
}
