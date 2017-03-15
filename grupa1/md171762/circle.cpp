class Circle : public Figure{    Point a;    double r;    virtual ostream& output(ostream& os) const {        return os << "Circle:" << a << ", r = " << r <<endl;    }public:    Circle(Point p, double _r) {        a = p;        r= _r;    }    virtual double area() {        return 3.14 * r * r;    }    virtual Figure* scale(double k) {      a.scale(k);      r=r*k;        return this;    }    virtual Figure* translate(double dx, double dy) {        a.translate(dx, dy);        return this;    }    virtual Figure* rotate(double alpha) {        a.rotate(alpha);        return this;    }};
