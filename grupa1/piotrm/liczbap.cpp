#include <iostream>

using namespace std;

int main()
{
    int n, t1, t2;
    cin>>n;

    for(int i=2; i*i<=n; i++){
        t1=n/i;

        if(t1*i==n){
                cout<< "zlozona" <<endl;
                return 0;
        }


    }
cout<< "pierwsza"<<endl;

    return 0;
}
