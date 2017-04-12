#include<iostream>
#include<vector>
using namespace std;

class Polynomial
{
    	vector<double> a;
      
public:
    	Polynomial() {}

    	double getA(int n) const {
		if (n >= a.size()) return 0;
		return a[n];
	}

    	void setA(int n, double x) {
		if (n >= a.size()) { a.resize(n+1,0); }
		a[n]=x;
	}
    
    	int deg() const {
		int d = a.size()-1;
		while (d>=0 && getA(d) == 0) { d--; }
		return d;
	}
    
    	Polynomial& operator+=(const Polynomial &q) {
		for (int i = 0; i <= deg() || i <= q.deg(); i++) {
			setA(i, getA(i) + q.getA(i));
		} 
		return *this;
	}

    	const Polynomial operator+(const Polynomial &q) const {
		Polynomial r(*this);
		r += q;
		return r;
	}

    	Polynomial& operator*=(const Polynomial &q) {
		for (int i = deg() + q.deg(); i >= 0; i--) {
			double x = 0;
			for (int j = 0; j <= i; j++) {
				x += getA(j) * q.getA(i-j);
			}
			setA(i, x);
		}
		return *this;
	}

    	const Polynomial operator*(const Polynomial &q) const {
		Polynomial r(*this);
		r *= q;
		return r;	
	}

    	Polynomial& operator-=(const Polynomial &q) {
		for (int i = 0; i <= deg() || i <= q.deg(); i++) {
			setA(i, getA(i) - q.getA(i));
		} 
		return *this;
	}
    	
	const Polynomial operator-(const Polynomial &q) const {
		Polynomial r(*this);
		r -= q;
		return r;	
	}

    	bool operator==(const Polynomial &q) const {
		for (int i = 0; i <= deg() || i <= q.deg(); i++) {
			if (getA(i) != q.getA(i)) return false;
		}
		return true;
	}

   	bool operator!=(const Polynomial &q) const {
		return !(*this == q);
	}

    	double v(double x) const {
		double r = 0;
		for (int i = deg(); i >= 0; i++) {
			r *= x;
			r += getA(i);
		}
		return r;
	}
    
    	friend ostream& operator<<(ostream&, const Polynomial&);        
};

ostream& operator<<(ostream& os, const Polynomial& p) {
	bool first = true;
	for (int i = p.deg(); i >= 0; i--) {
		if (!first) {
			if (p.getA(i) < 0) {
				os << " - " << (-p.getA(i));
			} else {
				os << " + " << p.getA(i);
			}
		} else {
			os << p.getA(i);
		}
		first = false;
		if (i > 1) {
			os << "x^" << i;
		} else if (i == 1) {
			os << "x";
		} else {
		}
	}
	return os;
}

void div(const Polynomial &W, const Polynomial &P, Polynomial &Q, Polynomial &R);

int main()
{
    Polynomial p, q;

    p.setA(2,3); p.setA(1,4); p.setA(0,-5);
    cout << p << endl;                         // wypisuje 3x^2 + 4x -5

    q.setA(1,2); q.setA(0,3);
    cout << q << endl;                         // wypisuje 2x + 3

    // wypisuje (3x^2+4x-5)*(2x+3)=6x^3 + 17x^2 + 2x - 15
    cout << "(" << p << ")*("<<q<<")="<<(p*q)<<endl;
}
