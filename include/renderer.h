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
    Point camera_pos;
    Point pixel00_loc; // Location of (0, 0) pixel
    Vector pixel_delta_x;
    Vector pixel_delta_y;
    int samples_num;

    void initialize();
public:
    
    Renderer(int image_width, int image_height, int samples_num = SAMPLES_NUM):
    image_width(image_width),
    image_height(image_height),
    samples_num(samples_num)
    {
        initialize();
    }


    void render(sf::Image& image, const Hittable& world) const;
};


