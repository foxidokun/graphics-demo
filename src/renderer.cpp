#include "renderer.h"
#include "material.h"
#include <cstdio>

// ---------------------------------------------------------------------------------------------------------------------
// Declarations
// ---------------------------------------------------------------------------------------------------------------------

static void draw_pixel(sf::Image& image, Color color, uint x, uint y, int samples_num);
static Vector pixel_sample_square(const Vector& pixel_delta_u, const Vector& pixel_delta_v);
static Color ray_color(const Ray& ray, const Hittable& world, uint depth);
static Point defocus_disk_sample(const Point& center, double radius, const Vector& u, const Vector& v);

// ---------------------------------------------------------------------------------------------------------------------
// Render self-configuration
// ---------------------------------------------------------------------------------------------------------------------

void Renderer::configure() {
    // vp means viewport
    double fov_rad = degrees_to_radians(vfov);
    double vp_height = 2 * tan(fov_rad / 2) * focus_dist;
    double vp_width = vp_height * (((double)image_width) / image_height); // preserve ratio

    // Camera basic vectors
    w = (lookfrom - lookat).norm(); // forward
    u = cross(vup, w).norm();       // left
    v = cross(w, u);                // up

    // Viewport sides
    Vector vp_horiz =  vp_width  * u;
    Vector vp_vert  = -vp_height * v;

    // Calculate pixel delta vectors
    pixel_delta_x = vp_horiz / image_width;
    pixel_delta_y = vp_vert / image_height;

    // (0, 0) pixel location
    pixel00_loc = lookfrom - focus_dist * w - vp_horiz / 2 - vp_vert / 2 + 0.5 * (pixel_delta_x + pixel_delta_y);

    if (defocus_angle > 0) {
        defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
    } else {
        defocus_radius = 0;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// Actual rendering
// ---------------------------------------------------------------------------------------------------------------------

void Renderer::render(sf::Image& image, const Hittable& world) const {
    #if PRINT_PROGRESS
        uint finished_rows_cnt = 0;
    #endif

    #pragma omp parallel for schedule(dynamic, 10) // dynamic for dynamic job stealing, 10 is batch size
    for (uint y = 0; y < image_height; ++y) {
        for (uint x = 0; x < image_width; ++x) {
            Point pixel_center = pixel00_loc + (x * pixel_delta_x) + (y * pixel_delta_y);
            Color pixel_color;

            for (int i = 0; i < samples_num; ++i) {
                Point pixel_sample = pixel_center + pixel_sample_square(pixel_delta_x, pixel_delta_y);
                Point ray_origin = defocus_disk_sample(lookfrom, defocus_radius, u, v);

                Vector ray_direction = pixel_sample - ray_origin;
                Ray ray(lookfrom, ray_direction);

                pixel_color += ray_color(ray, world, render_depth);
            }

            draw_pixel(image, pixel_color, x, y, samples_num);
        }

        #if PRINT_PROGRESS
            #pragma omp atomic
            ++finished_rows_cnt;

            printf("Finished %u out of %u rows\n", finished_rows_cnt, image_height);
        #endif
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// Ray math
// ---------------------------------------------------------------------------------------------------------------------

static Color ray_color(const Ray& ray, const Hittable& world, uint depth) {
    if (depth == 0) {
        return Color(0, 0, 0);
    }

    HitData hit;
    if (world.hit(ray, Interval(0.0001, infinity), hit)) {
        Ray scattered;
        Color color;
        if (hit.mat->scatter(ray, hit, color, scattered)) {
            return color * ray_color(scattered, world, depth - 1);
        }
        return Color(0, 0, 0);
    }

    Vector unit_direction = ray.direction.norm();
    double a = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - a) * White + a * LightBlue;
}


// ---------------------------------------------------------------------------------------------------------------------
// Small static helpers
// ---------------------------------------------------------------------------------------------------------------------

static void draw_pixel(sf::Image& image, Color color, uint x, uint y, int samples_num) {
    assert(x < image.getSize().x);
    assert(y < image.getSize().y);

    Interval allowed_intensivity(0, 1.0);
    color /= samples_num;

    // gamma correction
    double red   = sqrt(color.x);
    double green = sqrt(color.y);
    double blue  = sqrt(color.z);

    // clamping just in case & anti normalizing
    red   = allowed_intensivity.clamp(red)   * 255.0;
    green = allowed_intensivity.clamp(green) * 255.0;
    blue  = allowed_intensivity.clamp(blue)  * 255.0;

    image.setPixel(x, y, sf::Color(red, green, blue));
}

// ---------------------------------------------------------------------------------------------------------------------

/// Returns a random point in the square surrounding pixel
static Vector pixel_sample_square(const Vector& pixel_delta_x, const Vector& pixel_delta_y) {
    double px = random_double(-0.5, +0.5);
    double py = random_double(-0.5, +0.5);
    return (px * pixel_delta_x) + (py * pixel_delta_y);
}

// ---------------------------------------------------------------------------------------------------------------------

static Point defocus_disk_sample(const Point& center, double radius, const Vector& u, const Vector& v) {
    if (radius == 0) { // in case of disabled defocus
        return center;
    }

    double x = 1;
    double y = 1;

    while (x*x + y*y > 1) {
        x = random_double(-1, 1);
        y = random_double(-1, 1);
    }

    return center + (x * radius * u) + (y * radius * v);
}