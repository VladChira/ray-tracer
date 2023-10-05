#pragma once

#include "geometric_object.h"
namespace raytracer
{
    class Sphere : public GeometricObject
    {
    public:
        Eigen::Vector3f center;
        float radius;
        Eigen::AlignedBox3f aabb;

        Sphere();

        Sphere(const Eigen::Vector3f &c, float r, std::shared_ptr<raytracer::Material> mat);

        bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const;

        Eigen::AlignedBox3f bounding_box() const override;
    };

}