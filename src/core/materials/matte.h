#pragma once
#include "material.h"
#include "lambertian.h"
#include "geometric_object.h"
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
        Sampler *sampler;

    public:
        Matte()
        {
            sampler = new MultiJittered(10);
        }

        ~Matte()
        {
            delete sampler;
        }

        Matte(LambertianBRDF l)
        {
            diffuse_brdf = l;
            sampler = new MultiJittered(10);
        }

        Matte(double reflectance_coeff, Color3 color)
        {
            this->diffuse_brdf.set_kd(reflectance_coeff);
            this->diffuse_brdf.set_cd(color);
        }

        bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color3 &attenuation, raytracer::Ray &scattered)
        {
            return true;
        }
    };
}
