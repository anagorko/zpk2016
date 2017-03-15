#include <iostream>

using namespace std;

class Ulamek{

int l,m;

public:

    Ulamek(){};
    Ulamek(int _l, int _m){

    l = _l;
    m = _m;

    }

    int NWD(int x,int y ){
    int res;

    while(res!=0){
      res = x%y;
      x = y;
      y = res ;}
      return x;
    }

    Ulamek& operator+=(Ulamek w){

    int nwd =  NWD(m,w.m) ;

    l = (w.l*m+l*w.m) / nwd;

    m  =  m*w.m / nwd ;

    return *this;
    }

    Ulamek operator+(Ulamek w){

    Ulamek tmp = *this;

    tmp += w;

    return tmp;


    }

    friend istream& operator>>(istream& i, Ulamek &w);
    friend ostream& operator<<(ostream &i, Ulamek w);

};

istream& operator>>(istream& i, Ulamek &w){

i >> w.l;
char temp;
i >> temp;
i >> w.m;

};

ostream& operator<<(ostream& o, Ulamek w){

 o << w.l << "/" << w.m << endl;

 return o;


};


main(){

Ulamek a,b;

cin >> a;cin>>b;

cout << a+b;

}
