#include <cmath>

#include "vector.h"

// #####################################################################################################################
// Operators
// #####################################################################################################################

Vector operator*(const double& scalar, const Vector& vec) {
    Vector copy = vec;
    copy *= scalar;
    return copy;
}

Vector operator+(const Vector& lhs, const Vector& rhs) {
    Vector copy = lhs;
    copy += rhs;
    return copy;
}

Vector operator-(const Vector& lhs, const Vector& rhs) {
    Vector copy = lhs;
    copy -= rhs;
    return copy;
}

std::ostream& operator<<(std::ostream &out, const Vector &vec) {
    out << "Vector {x: " << vec.x << "; y: " << vec.y << "; z: " << vec.z << "}";
    return out;
}

Vector operator*(const Vector& lhs, const Vector& rhs) {
    return Vector(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}


// #####################################################################################################################
// General purpose methods
// #####################################################################################################################

Vector Vector::norm() const {
    double len = length();
    return *this / len;
}

double dot(const Vector& lhs, const Vector& rhs) {
    return lhs.x * rhs.x + 
           lhs.y * rhs.y +
           lhs.z * rhs.z;
}
