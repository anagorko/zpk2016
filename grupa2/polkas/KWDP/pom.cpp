#include <iostream>
#include <algorithm>

using namespace std;

main(){

int it=0;
int i;
int a=0;

while(i!=-1){
    cin >> i;
    if(i>a && i!=0){it++;}
    a=i;
}
cout<<it;
}
