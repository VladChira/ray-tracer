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

        virtual Point3 sample() const
        {
            return Point3(0, 0, 0);
        }

        virtual double pdf(const raytracer::Ray &r, const HitInfo &rec) const
        {
            return 0.0;
        }

        virtual Normal3 get_normal(const Point3 p) const
        {
            return Normal3(0, 0, 0);
        }

        std::shared_ptr<Material> material;
    };
}