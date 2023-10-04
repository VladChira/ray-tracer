#pragma once
#include <memory>

#include "utilities.h"
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

        virtual Eigen::Vector3f sample() const
        {
            return Eigen::Vector3f(0, 0, 0);
        }

        virtual float pdf(const raytracer::Ray &r, const HitInfo &rec) const
        {
            return 0.0;
        }

        virtual Eigen::Vector3f get_normal(const Eigen::Vector3f p) const
        {
            return Eigen::Vector3f(0, 0, 0);
        }

        std::shared_ptr<Material> material;
    };
}