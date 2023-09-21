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
#include "material.h"

static void render_preview_mode(const Scene& world);

int main() {
    const Lambertian material_ground = Lambertian(Color(0.8, 0.8, 0.0));
    const Lambertian material_center = Lambertian(Color(0.1, 0.2, 0.5));
    const Dielectric material_left   = Dielectric(Color(1.0, 1.0, 1.0), 1.5);
    const Metal material_right       = Metal(Color(0.8, 0.6, 0.2), 1.0);

    Scene world;

    Sphere lawn(Point( 0.0, -100.5, -1.0), 100.0, &material_ground);
    Sphere center(Point( 0.0,    0.0, -1.0),   0.5, &material_center);
    Sphere left(Point(-1.0,    0.0, -1.0),   0.5, &material_left);
    Sphere right(Point( 1.0,    0.0, -1.0),   0.5, &material_right);

    world.add(&lawn);
    world.add(&center);
    world.add(&left);
    world.add(&right);

    #if PREVIEW_MODE
        render_preview_mode(world);
    #else
        #error "Not implemented yet"
    #endif
}


static void render_preview_mode(const Scene& world) {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

	sf::Texture buffer;
	sf::Sprite bufferSprite;
    buffer.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    bufferSprite.setTexture(buffer);
    sf::Image image = buffer.copyToImage();

    Renderer render(WINDOW_WIDTH, WINDOW_HEIGHT);
    render.lookfrom = Point(-2, 2, 1);
    render.lookat   = Point(0,0,-1);
    render.vup      = Vector(0,1,0);
    render.vfov     = 20;

    render.defocus_angle = 10.0;
    render.focus_dist    = 3.4;
    render.configure();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }

        render.render(image, world);
        buffer.loadFromImage(image);

        window.draw(bufferSprite);

        // end the current frame
        window.display();
    }
}