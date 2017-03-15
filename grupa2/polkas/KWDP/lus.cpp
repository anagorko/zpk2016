#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
using namespace std;

main(){

string st;
cin >> st;
int siz =st.size();
string number = "";
for(int i=siz;i>0;i--){


number = number + st.substr(i-1,1);

}

int num = atoi(number.data());

cout << num;

}
