#include "graphics.h"
#include "config.h"

void draw_vector(sf::RenderWindow &window, const Coordinates &coords, const Vector &vec, const sf::Color &color) {
    double end_x = coords.pixel_from_coord_x(vec.x);
    double end_y = coords.pixel_from_coord_y(vec.y);

    sf::Vertex end_point = sf::Vertex(sf::Vector2f(end_x, end_y), color);

    // Draw main line
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(coords.x, coords.y), color),
            end_point
    };

    window.draw(line, 2, sf::Lines);

    // Calculate coordinates of two other tip points
    Vector opposite = -vec.normalize() / 5;
    Vector ortho = Vector(-opposite.y, opposite.x);

    Vector left_point = vec + 2 * opposite + ortho;
    Vector right_point = vec + 2 * opposite - ortho;

    sf::Vertex triangle[] = {
            sf::Vertex(sf::Vector2f(
                    coords.pixel_from_coord_x(left_point.x),
                    coords.pixel_from_coord_y(left_point.y)
                    ), color),
            sf::Vertex(sf::Vector2f(
                    coords.pixel_from_coord_x(right_point.x),
                    coords.pixel_from_coord_y(right_point.y)
                    ), color),
            end_point
    };

    window.draw(triangle, 3, sf::Triangles);
}

void draw_coordinates(sf::RenderWindow &window, const Coordinates &coords) {
    sf::Vertex x_axis[] =
            {
                    sf::Vertex(sf::Vector2f(0, coords.y), sf::Color::Red),
                    sf::Vertex(sf::Vector2f(WINDOW_WIDTH, coords.y), sf::Color::Red)
            };

    sf::Vertex y_axis[] =
            {
                    sf::Vertex(sf::Vector2f(coords.x, 0), sf::Color::Red),
                    sf::Vertex(sf::Vector2f(coords.x, WINDOW_HEIGHT), sf::Color::Red)
            };

    window.draw(x_axis, 2, sf::Lines);
    window.draw(y_axis, 2, sf::Lines);

    int min_x_coord = (int) -floor(coords.x / coords.scale_x);
    int max_x_coord = floor((WINDOW_WIDTH - coords.x) / coords.scale_x);

    for (int x = min_x_coord; x <= max_x_coord; ++x) {
        sf::CircleShape circle(TIP_RADIUS);

        circle.setPosition(coords.pixel_from_coord_x(x) - TIP_RADIUS, coords.y - TIP_RADIUS);
        circle.setFillColor(sf::Color::Red);

        window.draw(circle);
    }

    int min_y_coord = floor((WINDOW_HEIGHT - coords.y) / coords.scale_y);
    int max_y_coord = (int) -floor(coords.y / coords.scale_y);

    for (int y = min_y_coord; y <= max_y_coord; ++y) {
        sf::CircleShape circle(TIP_RADIUS);

        circle.setPosition(coords.x - TIP_RADIUS, coords.pixel_from_coord_y(y) - TIP_RADIUS);
        circle.setFillColor(sf::Color::Red);

        window.draw(circle);
    }
}