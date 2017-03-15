#include <iostream>
using namespace std;

int Factorial( int integer)
{
if( integer == 1) return 1;
else{return (integer * (Factorial(integer-1)));}
}

main()
{

int n,fac;
cin >> n;
if(n == 0){fac = 1 ;} else {fac = Factorial(n);}
cout<<fac;

}

