#pragma once

#include "utilities.h"
#include "texture_mapping2D.h"

namespace raytracer
{
    class SphericalMapping : public TextureMapping2D
    {
    public:
        Eigen::Vector2f map(const Eigen::Vector3f &point, unsigned int width, unsigned int height) const override
        {
            float theta = acos(point.y());
            float phi = atan2(point.x(), point.z());
            if (phi < 0.0)
                phi += (2 * pi);

            float u = phi * (1 / (2 * pi));
            float v = 1 - theta * inv_pi;

            return Eigen::Vector2f((int)((width - 1) * (1 - v)), (int)((height - 1) * u));
        }

        Eigen::Vector2f map(const HitInfo &rec, unsigned int width, unsigned int height) const override
        {
            return map(rec.p, width, height);
        }
    };
}