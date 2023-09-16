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

    Metal(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& ray_in, const HitData& hit, Color& attenuation, Ray& scattered) const final {
        Vector reflected = reflect(ray_in.direction.norm(), hit.normal);
        scattered = Ray(hit.p, reflected);
        attenuation = albedo;
        return true;
    }
};