#include <iostream>

using namespace std;

main(){

int n;
cin >> n;
double fin,val;
cin >> fin;
double prices[n];
for(int i=0;i<n;i++){ cin>>prices[i];}


int shot=0;
int where[n];
int a=0;

for(int i=0;i<n;i++){
val = *(prices+i);
if(val==fin){shot++; where[a]=i;a++;}
}

if(shot==0){cout<< -1 << " "<<-1; return 0;}
else if (shot== 1){cout<< where[0] +1 << " "<< where[0] +1; return 0;}
else {cout<< where[0] +1 << " "<< where[a-1] +1; return 0;}

}
