
#include <iostream>
#include <cmath>
using namespace std;

main(){

int n;
cin>>n;
int it = 0;
while(n>0){
    n = n / 2;
    it ++;
}


for(int i=0;i<it;i++){
        long power = pow(2,i);
        cout<<power<<endl;
}
}
