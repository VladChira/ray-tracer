#include "material.h"

#include "hit_info.h"

namespace raytracer
{
    class Emissive : public Material
    {
    public:
        float ls;
        Color ce;
        Emissive(float ls, Color ce)
        {
            this->ls = ls;
            this->ce = ce;
        }

        bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color &attenuation, raytracer::Ray &scattered) const override
        {
            return false;
        }

        Color emitted(const raytracer::Ray &r_in, const HitInfo &rec) const override
        {
            if ((-rec.normal).dot(r_in.direction) > 0.0)
                return ce * ls;
            return Color::black;
        }

        Color shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            if ((-rec.normal).dot(r_in.direction) > 0.0)
                return ce * ls;
            return Color::black;
        }

        Color get_Le(const raytracer::Ray &r_in, const HitInfo &rec) override
        {
            return ce * ls;
        }

        Color preview_shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            return Color::white;
        }
    };
}