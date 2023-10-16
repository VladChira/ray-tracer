#pragma once

#include "hit_info.h"
#include "utilities.h"

namespace raytracer
{
    class TextureMapping2D
    {
    public:
        virtual Eigen::Vector2f map(const HitInfo &rec, unsigned int width, unsigned int height) const = 0;

        virtual Eigen::Vector2f map(const Eigen::Vector3f &point, unsigned int weight, unsigned int height) const = 0;

        virtual ~TextureMapping2D() {}
    };
}