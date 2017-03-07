#include <iostream>
using namespace std;
 
int a;
int main() {
 
cin>>a;

for(int i=2; i<a;i++) {
	if(a%i==0){
		cout<<"złożona"<<endl;
		return 0;
	}
 
 }
 cout <<"pierwsza"<<endl;
return 0;
}
