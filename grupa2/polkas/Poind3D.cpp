#include<iostream>
#include<math.h>
using namespace std;

class Point
{
    double x,y,z;

public:
    Point() {}
    Point(double _x, double _y, double _z)
    {
        x=_x;
        y=_y;
        z=_z;
    }

    ~Point() {}

    void setX(double _x) { x=_x;}  // ...
    void setY(double _y) { y=_y;}  // ...
    void setZ(double _z) { z=_z;}  // ...

    double getX() {return x; }  // ...
    double getY() {return y;}  // ...
    double getZ() {return z; }  // ...

    double distanceTo(Point p) {
        return sqrt(pow(x - p.x,2) + pow(y - p.y,2)+ pow(z - p.z,2));
    }

    friend istream& operator>>(istream&, Point&);
};

ostream& operator<<(ostream &o, Point p)
{
    o << "(" << p.getX() << "," << p.getY() << "," << p.getZ() << ")";

    return o;
}

istream& operator>>(istream &i, Point &p)
{
    i >> p.x;
    i >> p.y;
    i >> p.z;

    return i;
}

double area(Point a, Point b, Point c)
{
    double ab = a.distanceTo(b);
    double bc = b.distanceTo(c);
    double ac = a.distanceTo(c);

    double p = (ab + bc + ac) / 2.0;

    return sqrt(p * (p - ab) * (p - ac) * (p - bc)); // wzór Herona
}

int main()
{
    Point p;
    Point q;
    Point r;

    cout << "Podaj wspolrzedne punktu p: ";
    cin >> p;
    cout << "Podaj wspolrzedne punktu q: ";
    cin >> q;
    cout << "Podaj wspolrzedne punktu r: ";
    cin >> r;

    cout << "Punkty: " << p << " " << q << " " << r << endl;
    cout << "Pole: " << area(p,q,r) << endl;

    return 0;
}
