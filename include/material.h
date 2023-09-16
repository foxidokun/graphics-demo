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

    bool scatter(const Ray& ray_in, const HitData& hit, Color& attenuation, Ray& scattered) const final;
};

class Metal: public Material {
public:
    Color albedo;
    double fuzz;

    Metal(const Color& albedo, double fuzz):
        albedo(albedo),
        fuzz(fuzz < 1 ? fuzz : 1)
        {}

    bool scatter(const Ray& ray_in, const HitData& hit, Color& attenuation, Ray& scattered) const final;
};

class Dielectric: public Material {
public:
    Color albedo;
    double ir; // Index of Refraction

    Dielectric(const Color& albedo,double index_of_refraction):
        albedo(albedo),
        ir(index_of_refraction)
        {}

    bool scatter(const Ray& ray_in, const HitData& hit, Color& attenuation, Ray& scattered) const final;
};