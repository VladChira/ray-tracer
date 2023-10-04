#pragma once
#include "geometric_object.h"

namespace raytracer
{
    class Triangle : public GeometricObject
    {
    public:
        Eigen::Vector3f v0, v1, v2;
        Eigen::Vector3f normal;
        AABB aabb;

        Triangle();

        Triangle(const Eigen::Vector3f &a, const Eigen::Vector3f &b, const Eigen::Vector3f &c, std::shared_ptr<Material> mat);

        bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const override;

        AABB bounding_box() const override;
    };

    // std::vector<std::shared_ptr<Triangle>> tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps, std::shared_ptr<Material> mat);
}