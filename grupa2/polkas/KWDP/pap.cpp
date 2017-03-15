#include <iostream>
#include <cmath>
using namespace std;

int main(){
    int n;
    cin >> n;
    int t[n];

    int sum=0;
    for(int i=0;i<n+1;i++){
        cin>>t[i];
        sum = sum + t[i]*pow(2,i);
    }
    cout<<sum+1<<endl;
}
