#include <SFML/Graphics.hpp>
#include <chrono>

#include "coordinates.h"
#include "vector.h"
#include "graphics.h"
#include "config.h"

static uint64_t epoch_millisec();

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    Coordinates coord(WINDOW_WIDTH / 2 , WINDOW_HEIGHT / 2, 30.0, -30.0);
    Vector x(2, 3);
    Vector y(-1, 2);
    Vector rotating(5, 0);
    Vector mousevector;

    uint64_t prev_time = epoch_millisec();
    bool is_button_pressed = false;

    // run the program as long as the window is open
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseButtonReleased:
                    is_button_pressed = false;
                    break;

                case sf::Event::MouseButtonPressed:
                    is_button_pressed = true;
                    mousevector = Vector(coord.x_from_pixel(event.mouseButton.x),
                                         coord.y_from_pixel(event.mouseButton.y));
                    break;

                case sf::Event::MouseMoved:
                    if (is_button_pressed) {
                        mousevector = Vector(coord.x_from_pixel(event.mouseMove.x),
                                             coord.y_from_pixel(event.mouseMove.y));
                    }
                    break;

                default:
                    break;
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // Rotate vector
        rotating.rotate_deg((epoch_millisec() - prev_time) / 10.0);
        prev_time = epoch_millisec();

        // Draw them all
        draw_coordinates(window, coord);
        draw_vector(window, coord, x);
        draw_vector(window, coord, y);
        draw_vector(window, coord, x+y, sf::Color::Green);
        draw_vector(window, coord, rotating, sf::Color::Blue);
        draw_vector(window, coord, mousevector  , sf::Color::Cyan);

        // end the current frame
        window.display();
    }

    return 0;
}

static uint64_t epoch_millisec() {
    namespace chrono = std::chrono;
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}