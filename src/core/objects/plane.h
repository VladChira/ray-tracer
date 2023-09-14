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
        std::shared_ptr<raytracer::Material> material;

        Plane();

        Plane(raytracer::Point3 point, raytracer::Normal3 n);

        Plane(raytracer::Point3 point, raytracer::Normal3 n, std::shared_ptr<raytracer::Material> mat);

        bool hit(const raytracer::Ray &r, double t_min, double t_max, HitInfo &rec) const;
    };
}