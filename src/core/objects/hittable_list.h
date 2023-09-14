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
            const raytracer::Ray &r, double t_min, double t_max, HitInfo &rec) const override;

    public:
        std::vector<std::shared_ptr<raytracer::GeometricObject>> objects;
    };
}