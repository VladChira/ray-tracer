#pragma once
#include <memory>
#include "material.h"
#include "lambertian.h"
#include "geometric_object.h"
#include "multijittered.h"
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
        std::shared_ptr<Sampler> sampler;

    public:
        Matte()
        {
            sampler = std::make_shared<MultiJittered>(50);
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
                sampler = std::make_shared<MultiJittered>(50);
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

        void set_sampler(std::shared_ptr<Sampler> s)
        {
            this->sampler = sampler;
            sampler->map_samples_to_sphere();
        }

        inline Vector3 random_in_unit_sphere() const
        {
            while (true)
            {
                auto p = Vector3(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1));
                if (p.LengthSquared() < 1)
                    return p;
            }
        }

        inline Vector3 random_unit_vector() const
        {
            return Normalize(random_in_unit_sphere());
        }

        /*
         *  Calculate a new ray that is randomly scattered from the hit point
         */
        bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color3 &attenuation, raytracer::Ray &scattered) const override
        {
            auto scatter_direction = rec.normal + random_unit_vector();
            if (NearZero(scatter_direction))
            {
                scatter_direction = rec.normal;
            }
            scattered = raytracer::Ray(rec.p + 0.00001 * scatter_direction, scatter_direction);
            attenuation = this->diffuse_brdf.cd * this->diffuse_brdf.kd;
            return true;
        }

        Color3 shade(const raytracer::Ray &r_in, const HitInfo &rec) const
        {
            auto wo = -r_in.direction;
            Color3 accColor(0, 0, 0);
            for (int i = 0; i < rec.world.lights.size(); i++)
            {
                auto wi = rec.world.lights[i]->get_direction(r_in, rec);
                double ndotwi = Dot(wi, rec.normal);
                if (ndotwi > 0.0)
                {
                    bool in_shadow = false;
                    Ray shadow_ray = Ray(rec.p, wi);
                    in_shadow = rec.world.lights[i]->in_shadow(shadow_ray, rec);
                    if (!in_shadow)
                    {
                        Color3 dif = diffuse_brdf.f(rec, wo, wi);
                        Color3 L = rec.world.lights[i]->L(r_in, rec);
                        double G = rec.world.lights[i]->G(r_in, rec);
                        Color3 f1 = dif * L * G;
                        accColor += (f1 * ndotwi / rec.world.lights[i]->pdf(r_in, rec));
                    }
                }
            }
            // Clamp the color to 1.0 for now. Perhaps HDR in the future?
            return Clamp(accColor, 0.0, 1.0);
        }
    };
}
