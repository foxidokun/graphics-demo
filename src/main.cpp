#include <SFML/Graphics.hpp>
#include <chrono>

#include "coordinates.h"
#include "vector.h"
#include "graphics.h"
#include "config.h"

static uint64_t epoch_millisec();

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

	sf::Texture buffer;
	sf::Sprite bufferSprite;
    buffer.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    bufferSprite.setTexture(buffer);
    sf::Image image = buffer.copyToImage();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
        }

        for (int x = 0; x < WINDOW_WIDTH; ++x) {
            for (int y = 0; y < WINDOW_HEIGHT; ++y) {
                auto color = Color(double(x)/(WINDOW_WIDTH-1), double(y)/(WINDOW_HEIGHT-1), 0);
                draw_pixel(image, color, x, y);
            }
        } 

        buffer.loadFromImage(image);

        window.draw(bufferSprite);

        // end the current frame
        window.display();
    }
}

static uint64_t epoch_millisec() {
    namespace chrono = std::chrono;
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}