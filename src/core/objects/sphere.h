#pragma once
#include "geometric_object.h"
#include "maths.h"
#include "aabb.h"
namespace raytracer
{
    class Sphere : public GeometricObject
    {
    public:
        raytracer::Point3 center;
        double radius;
        AABB aabb;

        Sphere();

        Sphere(const raytracer::Vector3 &c, double r, std::shared_ptr<raytracer::Material> mat);

        bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const;

        AABB bounding_box() const override;
    };

}