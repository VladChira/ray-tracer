#pragma once
#include <memory>
#include "material.h"
#include "lambertian.h"
#include "geometric_object.h"
#include "pure_random.h"
#include "multijittered.h"
namespace raytracer
{
    /**
     * The Matte surface is represented here with a diffuse Lambertian BRDF.
     * I chose not to add the ambient BRDF because it is completely detached
     * from reality and wouldn't really work with G.I. methods later
     */
    class Matte : public Material
    {
    public:
        LambertianBRDF diffuse_brdf;
        std::shared_ptr<Sampler> sampler;

    public:
        Matte()
        {
            sampler = std::make_shared<PureRandom>(50);
            sampler->map_samples_to_sphere();
        }

        ~Matte()
        {
        }

        Matte(LambertianBRDF l)
        {
            diffuse_brdf = l;
            if (sampler == NULL)
            {
                sampler = std::make_shared<PureRandom>(50);
                sampler->map_samples_to_sphere();
            }
        }

        Matte(double kd, Color3 cd)
        {
            this->diffuse_brdf.set_kd(kd);
            this->diffuse_brdf.set_cd(cd);
            if (sampler == NULL)
            {
                sampler = std::make_shared<MultiJittered>(50);
                sampler->map_samples_to_sphere();
            }
        }

        inline Vector3 random_on_hemisphere(const Vector3 &normal) const
        {
            auto on_unit_sphere = sampler->sample_sphere();
            if (Dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
                return on_unit_sphere;
            else
                return -on_unit_sphere;
        }

        /*
         *  Calculate a new ray that is randomly scattered from the hit point
         */
        bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color3 &attenuation, raytracer::Ray &scattered) const override
        {
            // auto scatter_direction = rec.normal + sampler->sample_sphere();
            auto scatter_direction = random_on_hemisphere(rec.normal);
            if (NearZero(scatter_direction))
            {
                scatter_direction = rec.normal;
            }
            scattered = raytracer::Ray(rec.p, scatter_direction);
            attenuation = this->diffuse_brdf.cd * this->diffuse_brdf.kd;
            return true;
        }

        void set_sampler(std::shared_ptr<Sampler> s)
        {
            this->sampler = sampler;
            sampler->map_samples_to_sphere();
        }

        // Color3 shade(const raytracer::Ray &r_in, const HitInfo &rec)
        // {

        // }
    };
}
