#pragma once
#include <memory>

#include "geometric_object.h"
#include "sampler.h"
namespace raytracer
{
    class Rectangle : public GeometricObject
    {
    public:
        Eigen::Vector3f p0;
        Eigen::Vector3f a, b;
        float a_len_sq, b_len_sq;
        Eigen::Vector3f normal;
        float area, inv_area;
        std::shared_ptr<Sampler> sampler;
        Eigen::AlignedBox3f aabb;

        Rectangle(const Eigen::Vector3f &p0, const Eigen::Vector3f &a, const Eigen::Vector3f &b, std::shared_ptr<raytracer::Material> mat);
        
        Rectangle(const Eigen::Vector3f &p0, const Eigen::Vector3f &a, const Eigen::Vector3f &b, const Eigen::Vector3f normal, std::shared_ptr<raytracer::Material> mat);

        bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const;

        Eigen::AlignedBox3f bounding_box() const override;

        Eigen::Vector3f sample() const override;

        float pdf(const raytracer::Ray &r, const HitInfo &rec) const override;

        Eigen::Vector3f get_normal(const Eigen::Vector3f p) const override;
    };

    std::vector<std::shared_ptr<GeometricObject>> create_box(const Eigen::Vector3f& a, const Eigen::Vector3f& b, std::shared_ptr<Material> mat);
}

    static Eigen::AlignedBox3f compute_aabb(Eigen::Vector3f p0, Eigen::Vector3f a, Eigen::Vector3f b);