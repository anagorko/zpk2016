#include <iostream>

using namespace std;

main(){

char ints[4] = "fox";
char * point = &ints[0];
char * point2 = new char[4];

point2 = &ints[0];

delete [] point2;

cout << *(ints+1)<<endl;
cout << ints<<endl;
cout << (int *) ints<<endl;
cout << &ints<<endl;
cout<< *(point+1)<<endl;
cout<< *point2 <<endl;
cout<< point2 <<endl;
cout<< (int *) point2<<endl ;
cout<< &point2 ;
}
