#pragma once
#include "maths.h"
#include "ray.h"

class HitInfo;

namespace raytracer
{
    class Material
    {
    public:
        virtual bool scatter(const raytracer::Ray &r_in, const HitInfo &rec, raytracer::Color3 &attenuation, raytracer::Ray &scattered) const = 0;
    };
}