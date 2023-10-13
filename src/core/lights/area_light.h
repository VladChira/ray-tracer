#pragma once
#include <memory>

#include "geometric_object.h"
#include "light.h"
#include "material.h"
namespace raytracer
{
    class AreaLight : public Light
    {
    private:
        std::shared_ptr<GeometricObject> object;
        std::shared_ptr<Material> emissive_mat;

    public:
        AreaLight();

        // Eigen::Vector3f get_direction(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) override;
        // Color L(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const override;
        // float G(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const override;
        
        float pdf(const Ray &r_in, const HitInfo &rec) const override;
        bool occluded(const Ray &r, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const override;

        Color Sample_Li(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi, float &pdf, bool &in_shadow) const override;

        Color L(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const;
        
        
        inline void set_object(std::shared_ptr<GeometricObject> source)
        {
            object = source;
            emissive_mat = source->material;
        }
    };
}