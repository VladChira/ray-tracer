#pragma once
#include "geometric_object.h"
#include "maths.h"

namespace raytracer
{
    class Sphere : public GeometricObject
    {
    public:
        raytracer::Point3 center;
        double radius;
        std::shared_ptr<raytracer::Material> material;

        Sphere();

        Sphere(const raytracer::Vector3 &c, double r, std::shared_ptr<raytracer::Material> mat);

        bool hit(const raytracer::Ray &r, double t_min, double t_max, HitInfo &rec) const;
    };

}