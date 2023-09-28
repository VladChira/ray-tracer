#pragma once
#include <memory>

#include "maths.h"
#include "aabb.h"
#include "geometric_object.h"
#include "sampler.h"
namespace raytracer
{
    class Rectangle : public GeometricObject
    {
    public:
        Point3 p0;
        Vector3 a, b;
        double a_len_sq, b_len_sq;
        Normal3 normal;
        double area, inv_area;
        std::shared_ptr<Sampler> sampler;
        AABB aabb;

        Rectangle(const Point3 &p0, const Vector3 &a, const Vector3 &b, std::shared_ptr<raytracer::Material> mat);
        
        Rectangle(const Point3 &p0, const Vector3 &a, const Vector3 &b, const Normal3 normal, std::shared_ptr<raytracer::Material> mat);

        bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const;

        AABB bounding_box() const override;

        Point3 sample() const override;

        double pdf(const raytracer::Ray &r, const HitInfo &rec) const override;

        Point3 get_normal(const Point3 p) const override;
    };

    std::vector<std::shared_ptr<GeometricObject>> create_box(const Point3& a, const Point3& b, std::shared_ptr<Material> mat);

}