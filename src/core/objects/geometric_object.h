#pragma once
#include <memory>
#include "maths.h"
#include "ray.h"
#include "material.h"
#include "aabb.h"

namespace raytracer
{
    class GeometricObject
    {
    public:
        virtual bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const = 0;
        virtual AABB bounding_box() const = 0;

        std::shared_ptr<raytracer::Material> material;
    };
}

class HitInfo
{
public:
    raytracer::Point3 p;
    raytracer::Normal3 normal;
    double t;
    std::shared_ptr<raytracer::Material> material;

    bool front_face;

    inline void set_face_normal(const raytracer::Ray &r, const raytracer::Normal3 &outward_normal)
    {
        front_face = raytracer::Dot(r.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};