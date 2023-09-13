#include "vector.h"
#include "primitives.h"

class Ray {
public:
    Point orig;
    Vector dir;

    Ray() {};
    Ray(const Point& origin, const Vector& direction):
        orig(origin),
        dir(direction)
        {};

    Point at(double t) const {
        return orig + t * dir;
    }
};