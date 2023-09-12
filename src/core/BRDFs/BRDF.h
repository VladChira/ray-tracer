#pragma once
#include "maths.h"
#include "geometric_object.h"

namespace raytracer
{
    class BRDF
    {
    public:
        // virtual Color3 f(const HitInfo &hi, const Vector3 &wi, const Vector3 &wo) const = 0;

        // virtual Color3 sample_f(const HitInfo &hi, Vector3 &wi, const Vector3 &wo) const = 0;

        // virtual Color3 sample_f(const HitInfo &hi, Vector3 &wi, const Vector3 &wo, double pdf) const = 0;

        // virtual Color3 tho(const HitInfo &hi, const Vector3 &wo) const = 0;
    };
}