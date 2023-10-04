#pragma once

#include <memory>

#include "utilities.h"
#include "geometric_object.h"

namespace raytracer
{
    class Sampler;
    class BRDF
    {
    public:
        virtual Color f(const HitInfo &hi, const Eigen::Vector3f &wi, const Eigen::Vector3f &wo) const = 0;

        virtual Color sample_f(const HitInfo &hi, const Eigen::Vector3f &wo, Eigen::Vector3f &wi, float &pdf) const = 0;

        virtual Color rho(const HitInfo &hi, const Eigen::Vector3f &wo) const = 0;

        std::shared_ptr<Sampler> sampler;
    };
}