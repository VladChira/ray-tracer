#pragma once
#include <vector>
#include <memory>
#include "geometric_object.h"

namespace raytracer
{
    class HittableList : public raytracer::GeometricObject
    {
    public:
        HittableList();
        HittableList(std::shared_ptr<raytracer::GeometricObject> object);

        void clear();
        void add(std::shared_ptr<raytracer::GeometricObject> object);

        virtual bool hit(
            const raytracer::Ray &r, Interval t_range, HitInfo &rec) const override;

        AABB bounding_box() const
        {
            return aabb;
        }

    public:
        std::vector<std::shared_ptr<raytracer::GeometricObject>> objects;
        AABB aabb;
    };
}