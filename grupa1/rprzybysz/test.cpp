class Plane {
public:
    Plane(Point A, Point B, Point C);
    moveTo(Point P);
    distanceFrom(Point P);
    parallelTo(Plane &P);
};
