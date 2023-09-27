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

        // This does not work with multithreading at the moment
        Point3 sample_point;
        Normal3 light_normal;
        Vector3 wi;

    public:
        AreaLight();

        Vector3 get_direction(const Ray &r_in, const HitInfo &rec) override;
        Color3 L(const Ray &r_in, const HitInfo &rec) const override;
        double G(const Ray &r_in, const HitInfo &rec) const override;
        double pdf(const Ray &r_in, const HitInfo &rec) const override;
        bool in_shadow(const Ray &r, const HitInfo &rec) const override;

        inline void set_object(std::shared_ptr<GeometricObject> source)
        {
            object = source;
            emissive_mat = source->material;
        }
    };
}