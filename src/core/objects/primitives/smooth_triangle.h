#pragma once
#include "geometric_object.h"

namespace raytracer
{
    class SmoothTriangle : public GeometricObject
    {
    public:
        Point3 v0, v1, v2;
        Normal3 n0, n1, n2;

        SmoothTriangle();

        SmoothTriangle(const Point3 &a, const Point3 &b, const Point3 &c, std::shared_ptr<Material> mat);

        bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const override;

        AABB bounding_box() const override;

        Normal3 interpolate_normal(const float beta, const float gamma) const;
    };

    std::vector<std::shared_ptr<SmoothTriangle>> tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps, std::shared_ptr<Material> mat);
}