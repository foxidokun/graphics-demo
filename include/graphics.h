#ifndef VECTOR_GRAPHICS_H
#define VECTOR_GRAPHICS_H

#include <SFML/Graphics.hpp>
#include "coordinates.h"
#include "vector.h"

void draw_vector(sf::RenderWindow& window, const Coordinates& coords, const Vector& vec,
    const sf::Color& color = sf::Color::White);
void draw_coordinates(sf::RenderWindow& window, const Coordinates& coords);

#endif //VECTOR_GRAPHICS_H
