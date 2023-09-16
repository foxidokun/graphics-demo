#pragma once

#include "scene.h"

class Material {
    public:
        virtual ~Material() = default;

        virtual bool scatter(const Ray& ray_in, const HitData& hit, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian: public Material {
public:
    Color albedo;

    Lambertian(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& ray_in, const HitData& hit, Color& attenuation, Ray& scattered) const final {
        Vector scatter_direction = hit.normal + random_unit_vector();
        if (scatter_direction.near_zero()) { // Fix bad random choices
            scatter_direction = hit.normal;
        }

        scattered = Ray(hit.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class Metal: public Material {
public:
    Color albedo;
    double fuzz;

    Metal(const Color& albedo, double fuzz):
        albedo(albedo),
        fuzz(fuzz < 1 ? fuzz : 1)
        {}

    bool scatter(const Ray& ray_in, const HitData& hit, Color& attenuation, Ray& scattered) const final {
        Vector reflected = reflect(ray_in.direction.norm(), hit.normal);
        scattered = Ray(hit.p, reflected + fuzz * random_unit_vector());
        attenuation = albedo;
        return (dot(scattered.direction, hit.normal) > 0.0);
    }
};