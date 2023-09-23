#pragma once
#include <memory>

#include "maths.h"
#include "ray.h"
#include "aabb.h"
#include "hit_info.h"

namespace raytracer
{
    class Material;
    class GeometricObject
    {
    public:
        virtual bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const = 0;
        virtual AABB bounding_box() const = 0;

        std::shared_ptr<Material> material;
    };
}