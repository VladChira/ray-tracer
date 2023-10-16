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

        Matte(float kd, std::shared_ptr<Texture> cd)
        {
            this->diffuse_brdf.set_kd(kd);
            this->diffuse_brdf.set_cd(cd);
            if (sampler == NULL)
            {
                sampler = std::make_shared<MultiJittered>(50);
                sampler->map_samples_to_sphere();
            }
        }

        Matte(float kd, Color cd)
        {
            auto tex = std::make_shared<ConstantTexture>(cd);
            this->diffuse_brdf.set_kd(kd);
            this->diffuse_brdf.set_cd(tex);
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
            attenuation = this->diffuse_brdf.cd->evaluate(rec) * this->diffuse_brdf.kd;
            return true;
        }

        Color shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            auto wo = -r_in.direction;
            Color pixel_color(0, 0, 0);
            // Estimate direct lighting for each light
            for (int i = 0; i < rec.world.lights.size(); i++)
            {
                Color Ld(0.0, 0.0, 0.0);

                // Sample light source with multiple importance sampling
                Eigen::Vector3f sample_point;
                Eigen::Vector3f light_normal;
                Eigen::Vector3f light_dir;
                bool in_shadow;
                Eigen::Vector3f wi;
                float light_pdf = 0.0, scattering_pdf = 0.0;
                Color Li = rec.world.lights[i]->Sample_Li(r_in, rec, sample_point, light_normal, wi, light_pdf, in_shadow);

                if (light_pdf > 0.0 && !Li.is_black())
                {

                    // Compute BRDF for light sample
                    Color f = diffuse_brdf.f(rec, wo, wi) * fabs(wi.dot(rec.normal));

                    // The pdf for lambertian is trivial
                    scattering_pdf = rec.normal.dot(wi) * inv_pi;

                    if (!f.is_black())
                    {
                        // Check visibility
                        if (in_shadow)
                            Li = Color::black;

                        // Add light's contribution to reflected radiance
                        if (!Li.is_black())
                        {
                            float weight = power_heuristic(1, light_pdf, 1, scattering_pdf);
                            Ld += f * Li * weight / light_pdf;
                        }
                    }
                }

                // Sample BRDF with multiple importance sampling

                // Sample scattered direction for surface interactions
                Color f = diffuse_brdf.sample_f(rec, wo, wi, scattering_pdf);
                float ndotwi = fabs(rec.normal.dot(wi));
                f *= ndotwi;

                if (!f.is_black() && scattering_pdf > 0.0)
                {
                    // Account for light contributions along sampled direction wi
                    float weight = 1;
                    light_pdf = rec.world.lights[i]->pdf(rec, wi); // over solid angle
                    if (light_pdf == 0.0)
                    {
                        pixel_color += Ld;
                        continue;
                    }
                    weight = power_heuristic(1, scattering_pdf, 1, light_pdf);

                    // Add light contribution from material sampling
                    Color Li = rec.world.lights[i]->L(r_in, rec, sample_point, light_normal, wi);
                    if (!Li.is_black())
                    {
                        Ld += f * Li * weight / scattering_pdf;
                    }
                }

                // Accumulate the contribution to each pixel
                pixel_color += Ld;
            }
            return pixel_color;
        }

        Color path_shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            Eigen::Vector3f wo = -r_in.direction;
            Eigen::Vector3f wi;
            float pdf;
            Color f = diffuse_brdf.sample_f(rec, wo, wi, pdf);
            float ndotwi = rec.normal.dot(wi);
            Ray reflected_ray(rec.p, wi);
            Color path_color = rec.world.tracer->trace_ray(reflected_ray, rec.world, rec.depth - 1);
            return f * path_color * ndotwi / pdf;
        }
    };
}
