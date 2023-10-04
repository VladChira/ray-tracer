#pragma once
#include "BRDF.h"
#include "utilities.h"
#include "multijittered.h"
#include "pure_random.h"
#include "jittered.h"
namespace raytracer
{
    /**
     * The simplest BRDF is the Lambertian BRDF. It represents perfect diffuse reflection.
     * Light is scattered equally in all directions (see 13.8 in RTFGO).
     */
    class LambertianBRDF : public BRDF
    {
    public:
        float kd;
        Color cd;

    public:
        LambertianBRDF()
        {
            kd = 1;
            cd = Color(0, 0, 0);
            sampler = std::make_shared<Jittered>(200);
            sampler->map_samples_to_hemisphere(1);
        }

        Color f(const HitInfo &hi, const Eigen::Vector3f &wi, const Eigen::Vector3f &wo) const override
        {
            return cd * kd * inv_pi;
        }

        inline Eigen::Vector3f random_cosine_direction() const
        {
            // Point2 s = sampler->sample_unit_square();
            float r1 = random_float();
            float r2 = random_float();

            float phi = 2 * pi * r1;
            float x = cos(phi) * sqrt(r2);
            float y = sin(phi) * sqrt(r2);
            float z = sqrt(1 - r2);

            return Eigen::Vector3f(x, y, z);
        }

        Color sample_f(const HitInfo &hi, const Eigen::Vector3f &wo, Eigen::Vector3f &wi, float &pdf) const override
        {
            Eigen::Vector3f w(hi.normal);
            Eigen::Vector3f v = (Eigen::Vector3f(0.0034f, 1.0f, 0.0071f).cross(w)).normalized();
            Eigen::Vector3f u = v.cross(w);

            Eigen::Vector3f sp = random_cosine_direction(); //sampler->sample_hemisphere();
            wi = (sp.x() * u + sp.y() * v + sp.z() * w).normalized();

            pdf = hi.normal.dot(wi) * inv_pi;
            return cd * kd * inv_pi;
        }

        Color rho(const HitInfo &hi, const Eigen::Vector3f &wo) const override
        {
            return cd * kd;
        }

        void set_kd(double _kd)
        {
            kd = _kd;
        }

        void set_cd(Color c)
        {
            cd = c;
        }
    };
}
