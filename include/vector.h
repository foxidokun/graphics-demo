#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <SFML/Graphics.hpp>
#include <cmath>

class Vector {
public:
    // ############
    // Fields
    // ############
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    // ############
    // (Con/De)structors
    // ############
    Vector() = default;

    Vector(double x, double y, double z):
        x(x),
        y(y),
        z(z)
        {}

    Vector(const double point[3]):
        x(point[0]),
        y(point[1]),
        z(point[2])
    {}

    // ############
    // Operators
    // ############

    Vector operator*(const double& scalar) const {
        return Vector(x * scalar, y * scalar, z * scalar);
    }

    Vector operator/(const double& scalar) const {
        return Vector(x / scalar, y / scalar, z / scalar);
    }

    Vector& operator*=(const double& scalar) {
        *this = *this * scalar;
        return *this;
    }

    Vector& operator/=(const double& scalar) {
        *this = *this / scalar;
        return *this;
    }

    Vector operator-() const {
        Vector res(-x, -y, -z);
        return res;
    }

    Vector& operator+=(const Vector& vec) {
        x += vec.x;
        y += vec.y;
        z += vec.z;

        return *this;
    }

    Vector& operator-=(const Vector& vec) {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;

        return *this;
    }

    // ###############
    // General purpose methods
    // ##############

    double length() const {
        return sqrt(length_square());
    }

    double length_square() const {
        return x * x + y * y + z * z;
    }

    Vector norm() const;
};

Vector operator*(const double& scalar, const Vector& vec);
Vector operator*(const Vector& lhs, const Vector& vec);
Vector operator+(const Vector& lhs, const Vector& rhs);
Vector operator-(const Vector& lhs, const Vector& rhs);

double dot(const Vector& lhs, const Vector& rhs);

std::ostream& operator<<(std::ostream &out, const Vector &vec);

#endif //VECTOR_VECTOR_H
