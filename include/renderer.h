#pragma once

#include <SFML/Graphics.hpp>
#include "coordinates.h"
#include "vector.h"
#include "config.h"
#include "scene.h"
#include "primitives.h"
#include <cassert>
#include <cstdio>
#include <stdint.h>
#include "interval.h"

class Renderer {
private:
    int image_width;
    int image_height;
    Point pixel00_loc; // Location of (0, 0) pixel
    Vector pixel_delta_x;
    Vector pixel_delta_y;
    Vector u, v, w; // Camera frame basis vectors

public:
    uint samples_num  = SAMPLES_NUM;
    uint render_depth = RENDER_DEPTH;
    double vfov       = 90;
    Point lookfrom    = Point(0, 0, 1);
    Point lookat      = Point(0, 0, 0);
    Vector vup        = Vector(0, 1, 0);

    Renderer(int image_width, int image_height):
    image_width(image_width),
    image_height(image_height)
    {
        configure();
    }

    void configure();
    void render(sf::Image& image, const Hittable& world) const;
};
