#include "vector.h"

class Color {
public:
    double r;
    double g;
    double b;

    Color() = default;
    Color(double r, double g, double b): 
        r(r),
        g(g),
        b(b)
        {};
};

class Point {
public:
    double x;
    double y;
    double z;

    Point() = default;

    Point(double x, double y, double z): 
        x(x),
        y(y),
        z(z)
        {};
};

static inline Point operator+(const Point& point, const Vector& vec) {
    return {point.x + vec.x, point.y + vec.y, point.z + vec.z};
}

static inline Point operator+(const Vector& vec, const Point& point) {
    return point + vec;
}

