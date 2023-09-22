#pragma once

#include "primitives.h"
#include "vector.h"

class Ray {
public:
    Point origin;
    Vector direction;

    Ray() = default;
    Ray(const Point& origin, const Vector& direction)
        : origin(origin)
        , direction(direction) {};

    Point at(double t) const {
        return origin + t * direction;
    }
};