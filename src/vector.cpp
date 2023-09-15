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

Vector random_reflection(const Vector& norm) {
    Vector reflected;

    while (true) {
        reflected = Vector::random(-1.0, +1.0);
        double len_aq = reflected.length_square();
        if (len_aq <= 1 && len_aq >= 0.01) {
            break;
        } 
    }

    reflected = reflected.norm();

    if (dot(norm, reflected) > 0.0) {
        return reflected;
    } else {
        return -reflected;
    }
}
