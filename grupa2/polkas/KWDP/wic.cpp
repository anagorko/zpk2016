
#include <iostream>
#include <algorithm>
using namespace std;

main()
{

int n;
cin>>n;

int numbers[n];

for(int i=0; i<n;i++){cin>>numbers[i];}

sort(numbers,numbers+n);

cout<<numbers[n-2];

}
