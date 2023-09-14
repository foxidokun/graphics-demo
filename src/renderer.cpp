#include "renderer.h"

static void draw_pixel(sf::Image& image, Color color, uint x, uint y, int samples_num);
static Vector pixel_sample_square(const Vector& pixel_delta_u, const Vector& pixel_delta_v);

static Color ray_color(const Ray& ray, const Hittable& world);

void Renderer::initialize() {
    camera_pos = Point(0, 0, 0);

    // Determine viewport dimensions.
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (((double)image_width)/image_height);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = Vector(viewport_width, 0, 0);
    auto viewport_v = Vector(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_x = viewport_u / image_width;
    pixel_delta_y = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left =
        camera_pos - Vector(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_x + pixel_delta_y);
}

void Renderer::render(sf::Image& image, const Hittable& world) const {
    #pragma omp parallel for
    for (uint x = 0; x < image_width; ++x) {
        for (uint y = 0; y < image_height; ++y) {
            Point pixel_center = pixel00_loc + (x * pixel_delta_x) + (y * pixel_delta_y);
            Color pixel_color;

            for (int i = 0; i < samples_num; ++i) {
                Point pixel_sample = pixel_center + pixel_sample_square(pixel_delta_x, pixel_delta_y);

                Vector ray_direction = pixel_sample - camera_pos;
                Ray ray(camera_pos, ray_direction);

                pixel_color += ray_color(ray, world); 
            }

            draw_pixel(image, pixel_color, x, y, samples_num);
        }
    } 
}

static Color ray_color(const Ray& ray, const Hittable& world) {
    HitData rec;
    if (world.hit(ray, Interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + White);
    }

    Vector unit_direction = ray.direction.norm();
    double a = 0.5*(unit_direction.y + 1.0);
    return (1.0-a)*White + a*LightBlue;
}

static void draw_pixel(sf::Image& image, Color color, uint x, uint y, int samples_num) {
    assert(x < image.getSize().x);
    assert(y < image.getSize().y);

    Interval allowed_intensivity(0, 1.0);
    color /= samples_num;

    uint red   = allowed_intensivity.clamp(color.x) * 255.0;
    uint green = allowed_intensivity.clamp(color.y) * 255.0;
    uint blue  = allowed_intensivity.clamp(color.z) * 255.0;

    image.setPixel(x, y, sf::Color(red, green, blue));
}

/// Returns a random point in the square surrounding a pixel at the origin.
static Vector pixel_sample_square(const Vector& pixel_delta_x, const Vector& pixel_delta_y) {
    auto px = -0.5 + random_double();
    auto py = -0.5 + random_double();
    return (px * pixel_delta_x) + (py * pixel_delta_y);
}
