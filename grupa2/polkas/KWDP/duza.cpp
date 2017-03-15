
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;


main(){

string c;
long double a,b;

cin >> a >> c >>b;

long double log_a = log(a);
long double log_b = log(b);


string strarray[] = {">", "<", "<=",">=","=="};


int d=0;
for(int i=0;i<5;i++){
if(strarray[i]==c){ d=i;}
}

bool res;

switch(d+1){
case 1 : { res = log_a > log_b ; break;}
case 2: { res = log_a < log_b ; break;}
case 3 : {res = log_a <= log_b ; break;}
case 4 : {res = log_a >= log_b ; break;}
case 5 : {res =  log_a == log_b ; break;}
}


if( res==1 ) {cout<<"TAK";} else {cout<< "NIE";}

}
