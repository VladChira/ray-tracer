#pragma once
#include "BRDF.h"
#include "maths.h"
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
            cd = Color3(0,0,0);
        }

        Color3 f(const HitInfo &hi, const Vector3 &wi, const Vector3 &wo)
        {
            return cd * kd * inv_pi;
        }

        Color3 sample_f(const HitInfo &hi, Vector3 &wi, const Vector3 &wo)
        {
            return Color3(0, 0, 0);
        }

        Color3 sample_f(const HitInfo &hi, Vector3 &wi, const Vector3 &wo, double pdf)
        {
            return Color3(0, 0, 0);
        }

        Color3 rho(const HitInfo &hi, const Vector3 &wo)
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

