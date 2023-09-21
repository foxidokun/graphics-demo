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

Vector cross(const Vector& u, const Vector& v) {
    return Vector(u.y * v.z - u.z * v.y,
                  u.z * v.x - u.x * v.z,
                  u.x * v.y - u.y * v.x);
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

Vector random_in_unit_disk() {
    while (true) {
        Vector tmp = Vector(random_double(-1,1), random_double(-1,1), 0);
        if (tmp.length_square() < 1) {
            return tmp;
        }
    }
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

/// @brief 
/// @param ray 
/// @param norm 
/// @param eta_rel -- relative eta of materials  
/// @return 
Vector refract(const Vector& ray, const Vector& norm, double eta_rel) {
    double cos_theta = fmin(dot(-ray, norm), 1.0);
    Vector r_out_perp = eta_rel * (ray + cos_theta*norm);
    Vector r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_square())) * norm;
    return r_out_perp + r_out_parallel;
}