#include "scene.h"

bool Sphere::hit(const Ray& ray, const Interval& render_interval, HitData& hit_data) const {
    // CO means Center->Origin vector
    Vector co = ray.origin - center;
    double a = ray.direction.length_square();
    double half_b = dot(co, ray.direction);
    double c = co.length_square() - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return false;
    }

    double sq_discr = sqrt(discriminant);

    // Find the nearest root that is good enough
    double root = (-half_b - sq_discr) / a;
    if (!render_interval.contains(root)) {
        root = (-half_b + sq_discr) / a;
        if (!render_interval.contains(root)) {
            return false;
        }
    }

    // Record and store some data about hit
    hit_data.t = root;
    hit_data.p = ray.at(hit_data.t);
    hit_data.mat = mat;
    Vector norm = (hit_data.p - center) / radius;
    hit_data.set_face_normal(ray, norm);

    return true;
}
