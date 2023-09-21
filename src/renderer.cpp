#include <cstdio>
#include "renderer.h"
#include "material.h"

static void draw_pixel(sf::Image& image, Color color, uint x, uint y, int samples_num);
static Vector pixel_sample_square(const Vector& pixel_delta_u, const Vector& pixel_delta_v);
static inline double gamma_correction(double value);
static Color ray_color(const Ray& ray, const Hittable& world, uint depth);
static Point defocus_disk_sample(const Point &center, const Vector& u, const Vector& v);

void Renderer::configure() {
    // Determine viewport dimensions.
    double theta = degrees_to_radians(vfov);
    double h = tan(theta/2);
    double viewport_height = 2 * h * focus_dist;
    double viewport_width = viewport_height * (((double)image_width)/image_height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    w = (lookfrom - lookat).norm();
    u = cross(vup, w).norm();
    v = cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    Vector viewport_u = viewport_width * u;
    Vector viewport_v = -viewport_height * v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_x = viewport_u / image_width;
    pixel_delta_y = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    Point viewport_upper_left =
        lookfrom - focus_dist * w - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_x + pixel_delta_y);

    double defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

void Renderer::render(sf::Image& image, const Hittable& world) const {
    #pragma omp parallel for
    for (uint y = 0; y < image_height; ++y) {
        for (uint x = 0; x < image_width; ++x) {
            Point pixel_center = pixel00_loc + (x * pixel_delta_x) + (y * pixel_delta_y);
            Color pixel_color;

            for (int i = 0; i < samples_num; ++i) {
                Point pixel_sample = pixel_center + pixel_sample_square(pixel_delta_x, pixel_delta_y);
                Point ray_origin = (defocus_angle <= 0) ?
                    lookfrom : defocus_disk_sample(lookfrom, defocus_disk_u, defocus_disk_v);

                Vector ray_direction = pixel_sample - lookfrom;
                Ray ray(lookfrom, ray_direction);

                pixel_color += ray_color(ray, world, render_depth); 
            }

            draw_pixel(image, pixel_color, x, y, samples_num);
        }

        #if PRINT_PROGRESS
            printf("Row %u finished \n", y);
        #endif
    } 
}

static Color ray_color(const Ray& ray, const Hittable& world, uint depth) {
    if (depth == 0) {
        return Color(0,0,0);
    }

    HitData hit;
    if (world.hit(ray, Interval(0.0001, infinity), hit)) {
        Ray scattered;
        Color attenuation;
        if (hit.mat->scatter(ray, hit, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return Color(0,0,0);
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

    double red   = gamma_correction(color.x);
    double green = gamma_correction(color.y);
    double blue  = gamma_correction(color.z);

    red   = allowed_intensivity.clamp(red)   * 255.0;
    green = allowed_intensivity.clamp(green) * 255.0;
    blue  = allowed_intensivity.clamp(blue)  * 255.0;

    image.setPixel(x, y, sf::Color(red, green, blue));
}

static inline double gamma_correction(double value) {
    return sqrt(value);
}

/// Returns a random point in the square surrounding a pixel at the origin.
static Vector pixel_sample_square(const Vector& pixel_delta_x, const Vector& pixel_delta_y) {
    double px = -0.5 + random_double();
    double py = -0.5 + random_double();
    return (px * pixel_delta_x) + (py * pixel_delta_y);
}

static Point defocus_disk_sample(const Point &center, const Vector& u, const Vector& v) {
    // Returns a random point in the camera defocus disk.
    Vector p = random_in_unit_disk();
    return center + (p.x * u) + (p.y * v);
}