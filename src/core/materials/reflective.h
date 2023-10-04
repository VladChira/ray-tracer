#pragma once
#include "material.h"
#include "perfect_specular.h"
#include "geometric_object.h"
#include "multijittered.h"

namespace raytracer
{
    class Reflective : public Material
    {
    public:
        PerfectSpecular p_spec_brdf;
        Sampler *sampler = NULL;
        float fuzz;

        Reflective()
        {
            sampler = new MultiJittered(10);
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
                sampler = new MultiJittered(10);
                sampler->map_samples_to_sphere();
            }
        }

        Reflective(float kr, Color cr, float fuzz)
        {
            this->p_spec_brdf.set_kr(kr);
            this->p_spec_brdf.set_cr(cr);
            this->fuzz = fuzz;
            if (sampler == NULL)
            {
                sampler = new MultiJittered(10);
                sampler->map_samples_to_sphere();
            }
        }

        bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color &attenuation, raytracer::Ray &scattered) const override
        {
            Eigen::Vector3f reflected = Reflect((r_in.direction).normalized(), rec.normal);
            Eigen::Vector3f dir = reflected + fuzz * sampler->sample_sphere();
            scattered = Ray(rec.p + 0.001 * dir, dir);
            attenuation = this->p_spec_brdf.cr * this->p_spec_brdf.kr;
            return (scattered.direction.dot(rec.normal) > 0);
        }

        Color shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            return Color(0, 0, 0);
        }

        Color preview_shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            return p_spec_brdf.cr;
        }
    };
}