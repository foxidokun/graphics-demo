#pragma once

#include "ray.h"
#include "primitives.h"
#include <cassert>

class HitData {
public:
    Point p;
    Vector normal;
    double t;
    bool front_face;

    void set_face_normal(const Ray& ray, const Vector& outward_normal) {
        // NOTE: the parameter `outward_normal` is assumed to have unit length.
        assert(abs(outward_normal.length() - 1.0) < 1e-5);
        
        // Sets the hit record normal vector.
        front_face = dot(ray.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Shape {
public:
    virtual ~Shape() = default;
    virtual bool hit(const Ray& ray, double ray_tmin, double ray_tmax, HitData& hit_data) const = 0;
};

class Sphere: public Shape {
public:
    Point center;
    double radius;

    Sphere(Point center, double radius):
        center(center),
        radius(radius)
        {};
    
    bool hit(const Ray& ray, double ray_tmin, double ray_tmax, HitData& hit_data) const override;
};