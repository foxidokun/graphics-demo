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

Vector random_unit_vector() {
    Vector random_vec;

    while (true) {
        random_vec = Vector::random(-1.0, +1.0);
        double len_aq = random_vec.length_square();
        if (len_aq <= 1 && len_aq >= 0.01) {
            break;
        } 
    }

    random_vec = random_vec.norm();

    return random_vec;
}

Vector random_reflection(const Vector& norm) {
    Vector reflected = random_unit_vector();

    if (dot(norm, reflected) > 0.0) {
        return reflected;
    } else {
        return -reflected;
    }
}

Vector reflect(const Vector& vec, const Vector& norm) {
    return vec - 2*dot(vec,norm)*norm;
}