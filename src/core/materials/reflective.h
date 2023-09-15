#pragma once
#include "material.h"
#include "perfect_specular.h"
#include "geometric_object.h"
#include "pure_random.h"

namespace raytracer
{
    class Reflective : public Material
    {
    public:
        PerfectSpecular p_spec_brdf;
        Sampler *sampler = NULL;
        double fuzz;

        Reflective()
        {
            sampler = new PureRandom(10);
            sampler->map_samples_to_sphere();
        }

        ~Reflective()
        {
            delete sampler;
        }

        Reflective(PerfectSpecular p_spec)
        {
            p_spec_brdf = p_spec;
            if (sampler == NULL)
            {
                sampler = new PureRandom(10);
                sampler->map_samples_to_sphere();
            }
        }

        Reflective(double kr, Color3 cr, double fuzz)
        {
            this->p_spec_brdf.set_kr(kr);
            this->p_spec_brdf.set_cr(cr);
            this->fuzz = fuzz;
            if (sampler == NULL)
            {
                sampler = new PureRandom(10);
                sampler->map_samples_to_sphere();
            }
        }

        bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color3 &attenuation, raytracer::Ray &scattered) const override
        {
            Vector3 reflected = Reflect(Normalize(r_in.direction), rec.normal);
            scattered = Ray(rec.p, reflected + fuzz * sampler->sample_sphere());
            attenuation = this->p_spec_brdf.cr * this->p_spec_brdf.kr;
            return (Dot(scattered.direction, rec.normal) > 0);
        }
    };
}