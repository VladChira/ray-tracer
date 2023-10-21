#pragma once

#include "utilities.h"
#include "texture_mapping2D.h"

namespace raytracer
{
    class SphericalMapping : public TextureMapping2D
    {
    public:
        Eigen::Vector2f map(const HitInfo &rec) const override
        {
            // float theta = acos(point.y());
            // float phi = atan2(point.x(), point.z());
            // if (phi < 0.0)
            //     phi += (2 * pi);

            // float u = phi * (1 / (2 * pi));
            // float v = 1 - theta * inv_pi;

            // return Eigen::Vector2f(u, v);

            return Eigen::Vector2f(0.0, 0.0);
        }
    };
}