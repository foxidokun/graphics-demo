#include "shapes.h"

bool Sphere::hit(const Ray& ray, double ray_tmin, double ray_tmax, HitData& hit_data) const {
    Vector oc = ray.origin - center;
    double a = ray.direction.length_square();
    double half_b = dot(oc, ray.direction);
    double c = oc.length_square() - radius*radius;
    double discriminant = half_b*half_b - a*c;
    
    if (discriminant < 0) {
        return false;
    }
    
    double sq_discr = sqrt(discriminant); 

    // // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sq_discr) / a;
    if (root <= ray_tmin || root >= ray_tmax) {
        root = (-half_b + sq_discr) / a;
        if (root <= ray_tmin || root >= ray_tmax) {
            return false;
        }
    }

    // Record and store some data about hit
    hit_data.t = root;
    hit_data.p = ray.at(hit_data.t);
    Vector outward_normal = (hit_data.p - center) / radius;
    hit_data.set_face_normal(ray, outward_normal);

    return true;
}
