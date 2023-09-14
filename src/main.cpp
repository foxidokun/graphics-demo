#include <SFML/Graphics.hpp>
#include <chrono>

#include "coordinates.h"
#include "vector.h"
#include "graphics.h"
#include "primitives.h"
#include "ray.h"
#include "config.h"
#include "scene.h"
#include "common.h"
#include "interval.h"

Color ray_color(const Ray& ray, const Hittable& world) {
    HitData rec;
    if (world.hit(ray, Interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + White);
    }

    Vector unit_direction = ray.direction.norm();
    double a = 0.5*(unit_direction.y + 1.0);
    return (1.0-a)*White + a*LightBlue;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)(WINDOW_WIDTH)/WINDOW_HEIGHT);
    Point camera_center(0, 0, 0);

    Vector viewport_u(viewport_width, 0, 0);
    Vector viewport_v(0, -viewport_height, 0);

    Vector pixel_delta_u = viewport_u / WINDOW_WIDTH;
    Vector pixel_delta_v = viewport_v / WINDOW_HEIGHT;

    Point viewport_upper_left = camera_center
                             - Vector(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    Point pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	sf::Texture buffer;
	sf::Sprite bufferSprite;
    buffer.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    bufferSprite.setTexture(buffer);
    sf::Image image = buffer.copyToImage();

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

        for (int x = 0; x < WINDOW_WIDTH; ++x) {
            for (int y = 0; y < WINDOW_HEIGHT; ++y) {
                Point pixel_center = pixel00_loc + (x * pixel_delta_u) + (y * pixel_delta_v);
                Vector ray_direction = pixel_center - camera_center;
                Ray r(camera_center, ray_direction);

                Color pixel_color = ray_color(r, world);   
                draw_pixel(image, pixel_color, x, y);
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