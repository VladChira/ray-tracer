#pragma once
#include "BRDF.h"
#include "maths.h"
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
        double kd;
        Color3 cd;

    public:
        LambertianBRDF()
        {
            kd = 1;
            cd = Color3(0, 0, 0);
            sampler = std::make_shared<Jittered>(200);
            sampler->map_samples_to_hemisphere(1);
        }

        Color3 f(const HitInfo &hi, const Vector3 &wi, const Vector3 &wo) const override
        {
            return cd * kd * inv_pi;
        }

        inline Point3 random_cosine_direction() const
        {
            // Point2 s = sampler->sample_unit_square();
            auto r1 = random_double();
            auto r2 = random_double();

            auto phi = 2 * pi * r1;
            auto x = cos(phi) * sqrt(r2);
            auto y = sin(phi) * sqrt(r2);
            auto z = sqrt(1 - r2);

            return Point3(x, y, z);
        }

        Color3 sample_f(const HitInfo &hi, const Vector3 &wo, Vector3 &wi, double &pdf) const override
        {
            Vector3 w(hi.normal);
            Vector3 v = Normalize(Cross(Vector3(0.0034f, 1.0f, 0.0071f), w));
            Vector3 u = Cross(v, w);

            Point3 sp = random_cosine_direction(); //sampler->sample_hemisphere();
            wi = Normalize(sp.x * u + sp.y * v + sp.z * w);

            pdf = Dot(hi.normal, wi) * inv_pi;
            return kd * cd * inv_pi;
        }

        Color3 rho(const HitInfo &hi, const Vector3 &wo) const override
        {
            return cd * kd;
        }

        void set_kd(double _kd)
        {
            kd = _kd;
        }

        void set_cd(Color3 c)
        {
            cd = c;
        }
    };
}
