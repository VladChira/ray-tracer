#pragma once

#include "hit_info.h"
#include "utilities.h"

namespace raytracer
{
    class TextureMapping2D
    {
    public:
        virtual Eigen::Vector2f map(const HitInfo &rec) const = 0;

        virtual ~TextureMapping2D() {}
    };
}