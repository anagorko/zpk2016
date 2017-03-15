#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

main(){

double r;
double pole;
double obwod;

cin>>r;

pole = M_PI * pow(r,2);
obwod = 2*M_PI*r;

cout<<fixed;
cout << setprecision(3) << pole << endl;
cout << setprecision(3) << obwod <<endl;
}
