#include "material.h"

// ---------------------------------------------------------------------------------------------------------------------
// Declarations
// ---------------------------------------------------------------------------------------------------------------------

static bool frenele_reflection(double cosine, double refraction_ratio);

// ---------------------------------------------------------------------------------------------------------------------
// Scatter functions
// 
// They calculate whether and where the ray will scatter. And it's color 
// ---------------------------------------------------------------------------------------------------------------------

bool Matte::scatter(const Ray&, const HitData& hit, Color& hit_color, Ray& scattered) const {
    // Matte distribution. p ~ cos (normal ^ ray)
    Vector scatter_direction = hit.normal + random_unit_vector();
    if (scatter_direction.near_zero()) { // Fix bad random choices to avoid NaN or inf in divides
        scatter_direction = hit.normal;
    }

    scattered = Ray(hit.p, scatter_direction);
    hit_color = color;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bool Metal::scatter(const Ray& ray_in, const HitData& hit, Color& hit_color, Ray& scattered) const {
    Vector reflected = reflect(ray_in.direction.norm(), hit.normal);
    scattered = Ray(hit.p, reflected + fuzz * random_unit_vector()); // Add some noise
    hit_color = color;
    return (dot(scattered.direction, hit.normal) > 0.0);
}

// ---------------------------------------------------------------------------------------------------------------------

bool Glass::scatter(const Ray& ray_in, const HitData& hit, Color& hit_color, Ray& scattered) const {
    hit_color = color;
    double refraction_ratio = hit.front_face ? (1.0 / refr_indx) : refr_indx;

    Vector unit_direction = ray_in.direction.norm();
    double cos_theta = fmin(dot(-unit_direction, hit.normal), 1.0); // just in case for sqrt in sin
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vector direction;

    if (cannot_refract || frenele_reflection(cos_theta, refraction_ratio)) {
        direction = reflect(unit_direction, hit.normal);
    } else {
        direction = refract(unit_direction, hit.normal, refraction_ratio);
    }

    scattered = Ray(hit.p, direction);
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------
// Internal static functions
// ---------------------------------------------------------------------------------------------------------------------

// probability based impolementation of partitial wave reflection / refraction.
// Uses Schlick's approximation for reflectance
// https://en.wikipedia.org/wiki/Schlick%27s_approximation
static bool frenele_reflection(double cosine, double refraction_ratio) {
    double r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
    r0 = r0 * r0;
    double r = r0 + (1 - r0) * pow((1 - cosine), 5);
    return r > random_double();
}