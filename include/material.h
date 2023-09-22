#pragma once

#include "scene.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& ray_in, const HitData& hit, Color& hit_color, Ray& scattered) const = 0;
};

class Lambertian: public Material {
public:
    Color color;

    Lambertian(const Color& color): color(color) {}

    bool scatter(const Ray& ray_in, const HitData& hit, Color& hit_color, Ray& scattered) const final;
};

class Metal: public Material {
public:
    Color color;
    double fuzz;

    Metal(const Color& color, double fuzz):
        color(color),
        fuzz(fuzz < 1 ? fuzz : 1)
        {}

    bool scatter(const Ray& ray_in, const HitData& hit, Color& hit_color, Ray& scattered) const final;
};

class Dielectric: public Material {
public:
    Color color;
    double refr_indx; // refraction index

    Dielectric(const Color& albedo, double refraction_index):
        color(color),
        refr_indx(refraction_index)
        {}

    bool scatter(const Ray& ray_in, const HitData& hit, Color& hit_color, Ray& scattered) const final;
};