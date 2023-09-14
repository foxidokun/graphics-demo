#include <SFML/Graphics.hpp>
#include <chrono>

#include "coordinates.h"
#include "vector.h"
#include "renderer.h"
#include "primitives.h"
#include "ray.h"
#include "config.h"
#include "scene.h"
#include "common.h"
#include "interval.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

	sf::Texture buffer;
	sf::Sprite bufferSprite;
    buffer.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    bufferSprite.setTexture(buffer);
    sf::Image image = buffer.copyToImage();

    Renderer render(WINDOW_WIDTH, WINDOW_HEIGHT);

    Scene world;
    Sphere main_sphere(Point(0,0,-1), 0.5);
    Sphere lawn(Point(0,-100.5,-1), 100);

    world.add(&main_sphere);
    world.add(&lawn);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
        }

        render.render(image, world);
        buffer.loadFromImage(image);

        window.draw(bufferSprite);

        // end the current frame
        window.display();
    }
}
