#ifndef VECTOR_GRAPHICS_H
#define VECTOR_GRAPHICS_H

#include <SFML/Graphics.hpp>
#include "coordinates.h"
#include "vector.h"
#include <cassert>
#include <cstdio>
#include <stdint.h>

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

static inline void draw_pixel(sf::Image& image, Color& color, uint x, uint y) {
    assert(x < image.getSize().x);
    assert(y < image.getSize().y);

    image.setPixel(x, y, sf::Color(color.r * 255.0, color.g * 255.0, color.b * 255.0));
}

#endif //VECTOR_GRAPHICS_H
