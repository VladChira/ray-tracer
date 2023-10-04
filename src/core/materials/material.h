#pragma once
#include "utilities.h"
#include "ray.h"
#include "hit_info.h"
#include "world.h"
#include "light.h"

namespace raytracer
{
    class Material
    {
    public:
        virtual bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color &attenuation, raytracer::Ray &scattered) const = 0;
        virtual Color shade (const raytracer::Ray &r_in, HitInfo &rec) = 0;

        virtual Color preview_shade (const raytracer::Ray &r_in, HitInfo &rec) = 0;

        virtual Color path_shade (const raytracer::Ray &r_in, HitInfo &rec)
        {
            return shade(r_in, rec);
        }

        virtual Color emitted(const raytracer::Ray &r_in, const HitInfo &rec) const
        {
            return Color(0, 0, 0);
        }

        virtual Color get_Le(const raytracer::Ray &r_in, const HitInfo &rec)
        {
            return Color(0, 0, 0);
        }
    };
}