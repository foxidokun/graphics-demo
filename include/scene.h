#pragma once

#include "ray.h"
#include "primitives.h"
#include "interval.h"
#include <cassert>

// To avoid cyclic import
class Material;

// #####################
// Data about single hit
// #####################

class HitData {
public:
    Point p;
    Vector normal;
    double t;
    bool front_face;
    const Material* mat;

    void set_face_normal(const Ray& ray, const Vector& outward_normal) {
        // NOTE: the parameter `outward_normal` is assumed to have unit length.
        assert(fabs(outward_normal.length() - 1.0) < 1e-5);
        
        // Sets the hit record normal vector.
        front_face = dot(ray.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// #####################
// Abstract Shape Class
// #####################

class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& ray, const Interval& render_interval, HitData& hit_data) const = 0;
};

// #####################
// Sphere
// #####################

class Sphere: public Hittable {
public:
    Point center;
    double radius;
    const Material* mat;

    Sphere(Point center, double radius, const Material *mat):
        center(center),
        radius(radius),
        mat(mat)
        {};
    
    bool hit(const Ray& ray, const Interval& render_interval, HitData& hit_data) const final;
};

// ########################
// Scene as list of objects
// ########################

class Scene: public Hittable {
private:
    std::vector<Hittable *> objects;

public:
    Scene() = default;
    Scene(Hittable* object) {
        add(object);
    }

    void add(Hittable* object) {
        objects.push_back(object);
    }

    bool hit(const Ray& ray, const Interval& render_interval, HitData& hit_data) const final;
};