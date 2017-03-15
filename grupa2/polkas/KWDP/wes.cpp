
#include <iostream>

using namespace std;

int nwd(int a,int b){
int r;
while(b!=0){
  r = a % b;
  a=b;
  b=r;
}
  return a;
}

main(){

int n;
cin >> n;

int inputs[n][2];

for(int i =0; i<n;i++){ cin>> inputs[i][0]; cin>> inputs[i][1];}


for(int i =0; i<n;i++){
int nrs=inputs[i][0];
int jump=inputs[i][1];
int NWD = nwd(nrs,jump);

cout << nrs/NWD <<endl;

}
}
