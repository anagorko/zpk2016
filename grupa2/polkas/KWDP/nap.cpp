#include <iostream>
using namespace std;

main()
{
int n;
cin>>n;
int tab[n];

for(int i=0; i<n;i++) {cin >> tab[i];} ;

for(int i=0; i<n;i=i+2) {cout << tab[i] << " ";} ;
cout<<endl;
for(int i=1; i<n;i=i+2) {cout << tab[i] << " ";} ;


}


