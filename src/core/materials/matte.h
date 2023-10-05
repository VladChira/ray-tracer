#pragma once
#include <memory>
#include "material.h"
#include "lambertian.h"
#include "geometric_object.h"
#include "multijittered.h"
#include "pure_random.h"
#include "tracer.h"

// #include "Tracy.hpp"

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

        Matte(float kd, Color cd)
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

        inline Eigen::Vector3f random_in_unit_sphere() const
        {
            while (true)
            {
                auto p = Eigen::Vector3f(random_float(-1, 1), random_float(-1, 1), random_float(-1, 1));
                if (p.squaredNorm() < 1)
                    return p;
            }
        }

        inline Eigen::Vector3f random_unit_vector() const
        {
            return (random_in_unit_sphere()).normalized();
        }

        /*
         *  Calculate a new ray that is randomly scattered from the hit point
         */
        bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color &attenuation, raytracer::Ray &scattered) const override
        {
            Eigen::Vector3f scatter_direction = rec.normal + random_unit_vector();
            if (NearZero(scatter_direction))
            {
                scatter_direction = rec.normal;
            }
            scattered = raytracer::Ray(rec.p + 0.0001 * scatter_direction, scatter_direction);
            attenuation = this->diffuse_brdf.cd * this->diffuse_brdf.kd;
            return true;
        }

        Color shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            auto wo = -r_in.direction;
            Color accColor(0, 0, 0);
            for (int i = 0; i < rec.world.lights.size(); i++)
            {
                Eigen::Vector3f sample_point;
                Eigen::Vector3f light_normal;
                Eigen::Vector3f light_dir;
                auto wi = rec.world.lights[i]->get_direction(r_in, rec, sample_point, light_normal, light_dir);
                float ndotwi = wi.dot(rec.normal);
                if (ndotwi > 0.0)
                {
                    bool in_shadow = false;
                    Ray shadow_ray = Ray(rec.p, wi);
                    in_shadow = rec.world.lights[i]->in_shadow(shadow_ray, rec, sample_point, light_normal, light_dir);
                    if (!in_shadow)
                    {
                        Color dif = diffuse_brdf.f(rec, wo, wi);
                        Color L = rec.world.lights[i]->L(r_in, rec, sample_point, light_normal, light_dir);
                        double G = rec.world.lights[i]->G(r_in, rec, sample_point, light_normal, light_dir);
                        Color f1 = L * dif * G;
                        accColor += (f1 * ndotwi / rec.world.lights[i]->pdf(r_in, rec));
                    }
                }
            }
            // Clamp the color to 1.0 for now. Perhaps HDR in the future?
            return Clamp(accColor, 0.0, 1.0);
        }

        Color path_shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            // ZoneScoped;
            Eigen::Vector3f wo = -r_in.direction;
            Eigen::Vector3f wi;
            float pdf;
            Color f = diffuse_brdf.sample_f(rec, wo, wi, pdf);
            float ndotwi = rec.normal.dot(wi);
            Ray reflected_ray(rec.p, wi);
            Color path_color = rec.world.tracer->trace_ray(reflected_ray, rec.world, rec.depth - 1);
            return f * path_color * ndotwi / pdf;
        }

        Color preview_shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            return diffuse_brdf.cd * diffuse_brdf.kd;
        }
    };
}
