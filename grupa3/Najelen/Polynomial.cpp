#include <iostream>
#include <vector>

using namespace std;

class Polynomial
{
    vector<double> a;

public:
    Polynomial(){};

    double getA(int n) const{
        Polynomial tmp = *this;
        int d = tmp.deg();
        if(n > d) return 0;
        else return a[n];
    };

    void setA(int n, double b){
        int r = n - a.size() + 1;
        while(r>0){
            a.push_back(0);
            r--;
        }
        a[n] = b;
    };

    int deg() const{
        int d = a.size()-1;
        while (a[d]==0){d--;}
        return d;
    };

    Polynomial& operator+=(const Polynomial &q){
        int qd = q.deg();
        int ad = a.size() - 1;

        if(qd > ad){
            for(int i = 0; i < (qd-ad); i++){
                a.push_back(0);
            }
        }
        for(int i = 0; i<=qd; i++){
            a[i]+= q.getA(i);
        }

        return *this;
    };

    const Polynomial operator+(const Polynomial &q) const{
        Polynomial tmp = *this;
        tmp += q;

        return tmp;
    };

    Polynomial& operator*=(const Polynomial &q){
        Polynomial tmp = *this;
        int qd = q.deg();
        int ad = tmp.deg();

        int d = qd + ad;

        vector<double> b;

        for(int i = 0; i <=d; i++){
            b.push_back(0);
        }

        for(int i = 0; i <(d - ad); i++){
            a.push_back(1);
        }


        for(int i = 0; i<=qd; i++){
            for(int k = 0; k <= ad; k++){
                b[i+k] += q.getA(i) * a[k];
            }
        }


        for(int i = 0; i<=d; i++){
            a[i] = b[i];
        }

        return *this;

    };

    const Polynomial operator*(const Polynomial &q) const{
        Polynomial tmp = *this;
        tmp *= q;

        return tmp;
    };

    Polynomial& operator-=(const Polynomial &q){
        int qd = q.deg();
        int ad = a.size() - 1;

        if(qd > ad){
            for(int i = 0; i < (qd-ad); i++){
                a.push_back(0);
            }
        }
        for(int i = 0; i<=qd; i++){
            a[i]-= q.getA(i);
        }
        return *this;
    };

    const Polynomial operator-(const Polynomial &q) const{
        Polynomial tmp = *this;
        tmp -= q;

        return tmp;
    };

    bool operator==(const Polynomial &q) const{
        Polynomial tmp = *this;
        int qd = q.deg();
        int ad = tmp.deg();
        int r = 0;

        if (qd != ad){return false;}
        else {
            for (int i = 0; i <= qd; i++){
                if (tmp.getA(i)!=q.getA(i)) {r++;}
            }
        }
        if (r != 0) {return false;}
        else {return true;}
    };

    bool operator!=(const Polynomial &q) const{
        Polynomial tmp = *this;
        int qd = q.deg();
        int ad = tmp.deg();
        int r = 0;

        if (qd != ad){return true;}
        else {
            for (int i = 0; i <= qd; i++){
                if (tmp.getA(i)!=q.getA(i)) {r++;}
            }
        }
        if (r != 0) {return true;}
        else {return false;}
    };

    double v(double x) const{
        int d = a.size() -1;
        int w = 0;

        vector<double> b;

        for(int i = 0; i<=d; i++){
            b.push_back(x);
        }

        for(int i = 0; i <= d; i++){
            for (int k = 1; k < i; k++){
                b[i] *= x;
            }
        }
        b[0] = 1;

        for(int i = 0; i<=d; i++){
            w += b[i] * a[i];
        }

        return w;
    };



    friend ostream& operator<<(ostream&, const Polynomial&);
};

void div(const Polynomial &W, const Polynomial &P, Polynomial &Q, Polynomial &R);

ostream& operator<<(ostream& o, const Polynomial& p){
    for(int i = p.deg(); i>1; i--) {
            if (p.getA(i) == 0) {
                if(i != p.deg()) {
                    if (p.getA(i-1) > 0) {cout <<"+ ";}
                }
                i--;
            }
            if (p.getA(i)!= 1) {cout << p.getA(i);}
            cout << "x^" << i << " ";
            if (p.getA(i-1) > 0) {cout <<"+ ";}
    }
    if(p.getA(1) != 0) {cout << p.getA(1) << "x ";}
    if(p.getA(0) > 0) {cout << "+ ";}
    if(p.getA(0) != 0) {cout << p.getA(0);}
    return o;
};

int main()
{
   Polynomial p, q;
   Polynomial r, s;

    p.setA(2,3); p.setA(1,4); p.setA(0,-5);
    cout << p << endl;                         // wypisuje 3x^2 + 4x -5


    q.setA(1,2); q.setA(0,3);
    cout << q << endl;  // wypisuje 2x + 3

    //r.setA(2,3); r.setA(1,4); r.setA(0,-5); r.setA(4,1);
    //cout << r << endl;

    //s.setA(1,1); s.setA(0,1);
    //cout << s << endl;

    // wypisuje (3x^2+4x-5)*(2x+3)=6x^3 + 17x^2 + 2x - 15
    cout << "(" << p << ")*("<<q<<")="<<(p*q)<<endl;
}
