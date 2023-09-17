#pragma once
#include "geometric_object.h"
#include "maths.h"

namespace raytracer
{
    class Plane : public GeometricObject
    {
    public:
        raytracer::Point3 a; // point through which the plane passes
        raytracer::Normal3 n; // normal to the plane

        Plane();

        Plane(raytracer::Point3 point, raytracer::Normal3 n);

        Plane(raytracer::Point3 point, raytracer::Normal3 n, std::shared_ptr<raytracer::Material> mat);

        bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const;

        AABB bounding_box() const override
        {
            return AABB();
        }
    };
}