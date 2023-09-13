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

    // ############
    // (Con/De)structors
    // ############
    Vector() = default;

    Vector(double x, double y):
        x(x),
        y(y)
        {}

    Vector(const double point[2]):
        x(point[0]),
        y(point[1])
    {}

    // ############
    // Operators
    // ############

    Vector operator*(const double& scalar) const {
        return Vector(x * scalar, y * scalar);
    }

    Vector operator/(const double& scalar) const {
        return Vector(x / scalar, y / scalar);
    }

    Vector& operator*=(const double& scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector& operator/=(const double& scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    Vector operator-() const {
        Vector res(-x, -y);
        return res;
    }

    Vector& operator+=(const Vector& vec) {
        x += vec.x;
        y += vec.y;

        return *this;
    }

    // ###############
    // General purpose methods
    // ##############

    void rotate_deg(double deg);
    void rotate_rad(double rad);

    double length() const {
        return sqrt(x * x + y * y);
    }

    Vector normalize() const;
};

Vector operator*(const double& scalar, const Vector& vec);
Vector operator+(const Vector& lhs, const Vector& rhs);
Vector operator-(const Vector& lhs, const Vector& rhs);

Vector normalized(const Vector& vec);

#endif //VECTOR_VECTOR_H
