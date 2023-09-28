#pragma once
#include "maths.h"
#include "ray.h"
#include "hit_info.h"
#include "world.h"
#include "light.h"

namespace raytracer
{
    class Material
    {
    public:
        virtual bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color3 &attenuation, raytracer::Ray &scattered) const = 0;
        virtual Color3 shade (const raytracer::Ray &r_in, HitInfo &rec) = 0;

        virtual Color3 preview_shade (const raytracer::Ray &r_in, HitInfo &rec) = 0;

        virtual Color3 path_shade (const raytracer::Ray &r_in, HitInfo &rec)
        {
            return shade(r_in, rec);
        }

        virtual Color3 emitted(const raytracer::Ray &r_in, const HitInfo &rec) const
        {
            return Color3(0, 0, 0);
        }

        virtual Color3 get_Le(const raytracer::Ray &r_in, const HitInfo &rec)
        {
            return Color3(0, 0, 0);
        }
    };
}