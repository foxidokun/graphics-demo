#pragma once

#include <SFML/Graphics.hpp>
#include "coordinates.h"
#include "vector.h"
#include "primitives.h"
#include <cassert>
#include <cstdio>
#include <stdint.h>
#include "interval.h"

static inline void draw_pixel(sf::Image& image, Color color, uint x, uint y, int samples_num) {
    assert(x < image.getSize().x);
    assert(y < image.getSize().y);

    Interval allowed_intensivity(0, 1.0);
    color /= samples_num;

    uint red   = allowed_intensivity.clamp(color.x) * 255.0;
    uint green = allowed_intensivity.clamp(color.y) * 255.0;
    uint blue  = allowed_intensivity.clamp(color.z) * 255.0;

    image.setPixel(x, y, sf::Color(red, green, blue));
}
