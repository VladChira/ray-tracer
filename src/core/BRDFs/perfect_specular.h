#include "BRDF.h"
#include "utilities.h"
namespace raytracer
{
    class PerfectSpecular : public BRDF
    {
    public:
        float kr;
        Color cr;

    public:
        PerfectSpecular()
        {
            kr = 1;
            cr = Color(0, 0, 0);
        }

        Color f(const HitInfo &hi, const Eigen::Vector3f &wi, const Eigen::Vector3f &wo) const override
        {
            return Color(0, 0, 0);
        }

        Color sample_f(const HitInfo &hi, const Eigen::Vector3f &wo, Eigen::Vector3f &wi, float &pdf) const override
        {
            return Color(0, 0, 0);
        }

        Color rho(const HitInfo &hi, const Eigen::Vector3f &wo) const override
        {
            return Color(0, 0, 0);
        }

        void set_kr(double _kr)
        {
            kr = _kr;
        }

        void set_cr(Color c)
        {
            cr = c;
        }
    };
}