#include "material.h"

static double reflectance(double cosine, double refraction_ratio);

bool Lambertian::scatter(const Ray& ray_in, const HitData& hit, Color& attenuation, Ray& scattered) const {
    Vector scatter_direction = hit.normal + random_unit_vector();
    if (scatter_direction.near_zero()) { // Fix bad random choices
        scatter_direction = hit.normal;
    }

    scattered = Ray(hit.p, scatter_direction);
    attenuation = albedo;
    return true;
}

bool Metal::scatter(const Ray& ray_in, const HitData& hit, Color& attenuation, Ray& scattered) const {
    Vector reflected = reflect(ray_in.direction.norm(), hit.normal);
    scattered = Ray(hit.p, reflected + fuzz * random_unit_vector());
    attenuation = albedo;
    return (dot(scattered.direction, hit.normal) > 0.0);
}

bool Dielectric::scatter(const Ray& ray_in, const HitData& hit, Color& attenuation, Ray& scattered) const {
    attenuation = albedo;
    double refraction_ratio = hit.front_face ? (1.0 / ir) : ir;

    Vector unit_direction = ray_in.direction.norm();
    double cos_theta = fmin(dot(-unit_direction, hit.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vector direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
        direction = reflect(unit_direction, hit.normal);
    } else {
        direction = refract(unit_direction, hit.normal, refraction_ratio);
    }

    scattered = Ray(hit.p, direction);
    return true;
}

static double reflectance(double cosine, double refraction_ratio) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}