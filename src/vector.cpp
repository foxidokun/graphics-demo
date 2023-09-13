#include <cmath>

#include "vector.h"

// #####################################################################################################################
// Operators
// #####################################################################################################################

Vector operator*(const double& scalar, const Vector& vec) {
    return Vector(vec.x * scalar, vec.y * scalar);
}

Vector operator+(const Vector& lhs, const Vector& rhs) {
    Vector res(lhs.x + rhs.x, lhs.y + rhs.y);
    return res;
}

Vector operator-(const Vector& lhs, const Vector& rhs) {
    Vector res(lhs.x - rhs.x, lhs.y - rhs.y);
    return res;
}

// #####################################################################################################################
// General purpose methods
// #####################################################################################################################

void Vector::rotate_rad(double rad) {
    double _cos = cos(rad);
    double _sin = sin(rad);

    double old_x = x;
    x = x * _cos -     y * _sin;
    y = y * _cos + old_x * _sin;
}

void Vector::rotate_deg(double deg) {
    rotate_rad(M_PI / 180 * deg);
}

Vector Vector::normalize() const {
    double len = length();
    return *this / len;
}