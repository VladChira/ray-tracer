#pragma once
#include "material.h"
#include "lambertian.h"
#include "geometric_object.h"
#include "pure_random.h"
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
        Sampler *sampler;

    public:
        Matte()
        {
            sampler = new PureRandom(10);
            sampler->map_samples_to_sphere();
        }

        ~Matte()
        {
            delete sampler;
        }

        Matte(LambertianBRDF l)
        {
            diffuse_brdf = l;
            if (sampler == NULL)
            {
                sampler = new PureRandom(10);
                sampler->map_samples_to_sphere();
            }
        }

        Matte(double kd, Color3 cd)
        {
            this->diffuse_brdf.set_kd(kd);
            this->diffuse_brdf.set_cd(cd);
            if (sampler == NULL)
            {
                sampler = new PureRandom(10);
                sampler->map_samples_to_sphere();
            }

        }

        bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color3 &attenuation, raytracer::Ray &scattered) const override
        {
            auto scatter_direction = rec.normal + sampler->sample_sphere();
            if(NearZero(scatter_direction))
            {
                scatter_direction = rec.normal;
            }
            scattered = raytracer::Ray(rec.p, scatter_direction);
            attenuation = this->diffuse_brdf.cd * this->diffuse_brdf.kd;
            return true;
        }

        void set_sampler(Sampler *s)
        {
            delete sampler;
            this->sampler = sampler;
            sampler->map_samples_to_sphere();
        }

        // Color3 shade(const raytracer::Ray &r_in, const HitInfo &rec)
        // {
            
        // }
        
    };
}
