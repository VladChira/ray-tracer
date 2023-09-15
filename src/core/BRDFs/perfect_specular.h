#include "BRDF.h"

namespace raytracer
{
    class PerfectSpecular : public BRDF
    {
    public:
        double kr;
        Color3 cr;

    public:
        PerfectSpecular()
        {
            kr = 1;
            cr = Color3(0, 0, 0);
        }

        Color3 f(const HitInfo &hi, const Vector3 &wi, const Vector3 &wo) const override
        {
            return Color3(0, 0, 0);
        }

        Color3 sample_f(const HitInfo &hi, Vector3 &wi, const Vector3 &wo) const override
        {
            return Color3(0, 0, 0);
        }

        Color3 sample_f(const HitInfo &hi, Vector3 &wi, const Vector3 &wo, double pdf) const override
        {
            return Color3(0, 0, 0);
        }

        Color3 rho(const HitInfo &hi, const Vector3 &wo) const override
        {
            return Color3(0, 0, 0);
        }

        void set_kr(double _kr)
        {
            kr = _kr;
        }

        void set_cr(Color3 c)
        {
            cr = c;
        }
    };
}