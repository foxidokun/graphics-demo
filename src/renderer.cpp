#include "renderer.h"
#include "material.h"
#include <cstdio>

// ---------------------------------------------------------------------------------------------------------------------
// Declarations
// ---------------------------------------------------------------------------------------------------------------------

static void draw_pixel(sf::Image& image, Color color, uint x, uint y, int samples_num);
static Vector pixel_sample_square(const Vector& pixel_delta_u, const Vector& pixel_delta_v);
static Color ray_color(const Ray& ray, const Hittable& world, uint depth);
static Point defocus_disk_sample(const Point& center, const Vector& u, const Vector& v);

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

    // Calculate pixeldelta vectors
    pixel_delta_x = vp_horiz / image_width;
    pixel_delta_y = vp_vert / image_height;

    // Calculate the location of the upper left pixel.
    pixel00_loc = lookfrom - focus_dist * w - vp_horiz / 2 - vp_vert / 2 + 0.5 * (pixel_delta_x + pixel_delta_y);

    double defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

// ---------------------------------------------------------------------------------------------------------------------
// Actual rendering
// ---------------------------------------------------------------------------------------------------------------------

void Renderer::render(sf::Image& image, const Hittable& world) const {
    #if PRINT_PROGRESS
        uint finished_rows_cnt = 0;
    #endif

    #pragma omp parallel for schedule(dynamic, 10)
    for (uint y = 0; y < image_height; ++y) {
        for (uint x = 0; x < image_width; ++x) {
            Point pixel_center = pixel00_loc + (x * pixel_delta_x) + (y * pixel_delta_y);
            Color pixel_color;

            for (int i = 0; i < samples_num; ++i) {
                Point pixel_sample = pixel_center + pixel_sample_square(pixel_delta_x, pixel_delta_y);
                Point ray_origin
                    = (defocus_angle <= 0) ? lookfrom : defocus_disk_sample(lookfrom, defocus_disk_u, defocus_disk_v);

                Vector ray_direction = pixel_sample - lookfrom;
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

    double red   = sqrt(color.x);
    double green = sqrt(color.y);
    double blue  = sqrt(color.z);

    red   = allowed_intensivity.clamp(red)   * 255.0;
    green = allowed_intensivity.clamp(green) * 255.0;
    blue  = allowed_intensivity.clamp(blue)  * 255.0;

    image.setPixel(x, y, sf::Color(red, green, blue));
}

// ---------------------------------------------------------------------------------------------------------------------

/// Returns a random point in the square surrounding a pixel at the origin.
static Vector pixel_sample_square(const Vector& pixel_delta_x, const Vector& pixel_delta_y) {
    double px = -0.5 + random_double();
    double py = -0.5 + random_double();
    return (px * pixel_delta_x) + (py * pixel_delta_y);
}

// ---------------------------------------------------------------------------------------------------------------------

static Point defocus_disk_sample(const Point& center, const Vector& u, const Vector& v) {
    Vector p;

    while (true) {
        p = Vector(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_square() <= 1) {
            break;
        }
    }

    return center + (p.x * u) + (p.y * v);
}