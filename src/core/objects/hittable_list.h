#pragma once
#include <vector>
#include <memory>
#include "geometric_object.h"

namespace raytracer
{
    class HittableList : public raytracer::GeometricObject
    {
    public:
        HittableList() {}
        HittableList(std::shared_ptr<raytracer::GeometricObject> object) { add(object); }

        void clear() { objects.clear(); }
        void add(std::shared_ptr<raytracer::GeometricObject> object) { objects.push_back(object); }

        virtual bool hit(
            const raytracer::Ray &r, double t_min, double t_max, raytracer::hit_info &rec) const override;

    public:
        std::vector<std::shared_ptr<raytracer::GeometricObject>> objects;
    };

    bool HittableList::hit(const raytracer::Ray &r, double t_min, double t_max, raytracer::hit_info &rec) const
    {
        raytracer::hit_info temp_rec;
        bool hit_anything = false;
        auto closest_so_far = t_max;

        for (const auto &object : objects)
        {
            if (object->hit(r, t_min, closest_so_far, temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
}