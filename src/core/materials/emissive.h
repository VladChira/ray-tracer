#include "material.h"

#include "hit_info.h"
#include "maths.h"

namespace raytracer
{
    class Emissive : public Material
    {
    public:
        double ls;
        Color3 ce;
        Emissive(double ls, Color3 ce)
        {
            this->ls = ls;
            this->ce = ce;
        }

        bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color3 &attenuation, raytracer::Ray &scattered) const override
        {
            return false;
        }

        Color3 emitted(const raytracer::Ray &r_in, const HitInfo &rec) const override
        {
            if (Dot(-rec.normal, r_in.direction) > 0.0)
                return ls * ce;
            return Color3::black;
        }

        Color3 shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            if (Dot(-rec.normal, r_in.direction) > 0.0)
                return ls * ce;
            return Color3::black;
        }

        Color3 get_Le(const raytracer::Ray &r_in, const HitInfo &rec) override
        {
            return ls * ce;
        }

        Color3 preview_shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            return Color3::white;
        }
    };
}