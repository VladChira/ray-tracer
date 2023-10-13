#pragma once
#include "utilities.h"
#include "ray.h"
#include "world.h"

namespace raytracer
{
    class Light
    {
    public:
        // virtual Eigen::Vector3f get_direction(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) = 0;
        // virtual Color L(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const = 0;
        // virtual float G(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const = 0;
        
        virtual Color Sample_Li(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi, float &pdf, bool &in_shadow) const = 0;

        
        virtual float pdf(const Ray &r_in, const HitInfo &rec) const = 0;
        virtual bool occluded(const Ray &r, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const = 0;
    };
}