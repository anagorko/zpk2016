#include <iostream>

using namespace std;

int n;

int main()
{
    int a;
    cin >> a;
    for(int i=2;i<a;i++)
    {
        if(a%i==0)
        {
            cout << "zlozona"<< endl;
            return 0;
        }
    }
    cout << "pierwsza"<< endl;
    return 0;
}
