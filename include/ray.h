#include "vector.h"
#include "primitives.h"

class Ray {
public:
    Point origin;
    Vector direction;

    Ray() {};
    Ray(const Point& origin, const Vector& direction):
        origin(origin),
        direction(direction)
        {};

    Point at(double t) const {
        return origin + t * direction;
    }
};