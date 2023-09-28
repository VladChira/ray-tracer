#pragma once

#include <memory>

#include "maths.h"
#include "geometric_object.h"

namespace raytracer
{
    class Sampler;
    class BRDF
    {
    public:
        virtual Color3 f(const HitInfo &hi, const Vector3 &wi, const Vector3 &wo) const = 0;

        virtual Color3 sample_f(const HitInfo &hi, const Vector3 &wo, Vector3 &wi, double &pdf) const = 0;

        virtual Color3 rho(const HitInfo &hi, const Vector3 &wo) const = 0;

        std::shared_ptr<Sampler> sampler;
    };
}