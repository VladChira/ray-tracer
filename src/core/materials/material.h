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
        virtual Color3 shade (const raytracer::Ray &r_in, const HitInfo &rec) const = 0;
    };
}