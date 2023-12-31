#pragma once
#include <memory>

#include <Eigen/Geometry>

#include "utilities.h"
#include "ray.h"
#include "hit_info.h"
#include "interval.h"
#include "transform.h"

namespace raytracer
{
    class Material;
    class GeometricObject
    {
    public:
        virtual bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const = 0;
        virtual Eigen::AlignedBox3f bounding_box() const = 0;

        virtual Eigen::Vector3f sample() const
        {
            return Eigen::Vector3f(0, 0, 0);
        }

        virtual float pdf(const raytracer::Ray &r, const HitInfo &rec) const
        {
            return 0.0;
        }

        virtual Eigen::Vector3f get_normal(const Eigen::Vector3f p) const
        {
            return Eigen::Vector3f(0, 0, 0);
        }

        virtual float pdf_solid_angle(const HitInfo &rec, const Eigen::Vector3f &wi) const
        {
            return 0.0;
        }

        void set_transform(Transform *t)
        {
            this->transform = t;
        }

        std::shared_ptr<Material> material;
        bool visible_to_camera = true;
        Transform *transform = nullptr;
    };
}