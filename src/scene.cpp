#include "scene.h"
#include "material.h"

bool Scene::hit(const Ray& ray, const Interval& render_interval, HitData& hit_data) const {
    HitData tmp_data;
    bool hit_anything = false;
    double closest_hit = render_interval.max;
    const double min_render = render_interval.min;

    for (uint i = 0; i < objects.size(); ++i) {
        if (objects[i]->hit(ray, Interval(min_render, closest_hit), tmp_data)) {
            hit_anything = true;
            closest_hit = tmp_data.t;
            hit_data = tmp_data;
        }
    }

    return hit_anything;
}

Scene::~Scene() {
    for (size_t i = 0; i < objects.size(); ++i) {
        delete (objects[i]);
    }

    for (size_t i = 0; i < materials.size(); ++i) {
        delete (materials[i]);
    }
}