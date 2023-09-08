#ifndef VECTOR_COORDINATES_H
#define VECTOR_COORDINATES_H

#include "vector.h"

class Coordinates {
public:
    double x = 0;
    double y = 0;

    double scale_x = 1.0;
    double scale_y = 1.0;

    Coordinates(double x, double y, double scale_x, double scale_y):
        x (x),
        y (y),
        scale_x (scale_x),
        scale_y (scale_y)
        {}

    Coordinates(double x, double y):
        x (x),
        y (y)
        {}

    Coordinates() = default;

    double pixel_from_coord_x(double x_coord) const {
        return x + x_coord * scale_x;
    }

    double pixel_from_coord_y(double y_coord) const {
        return y + y_coord * scale_y;
    }

    double x_from_pixel(double x_pixel) const {
        return (x_pixel - x) / scale_x;
    }

    double y_from_pixel(double y_pixel) const {
        return (y_pixel - y) / scale_y;
    }
};

#endif //VECTOR_COORDINATES_H
