#pragma once

#include "utilities.h"
#include "texture_mapping2D.h"

namespace raytracer
{
    class UVMapping : public TextureMapping2D
    {
    public:
        UVMapping()
        {
            scale_u = 1.0;
            scale_v = 1.0;
        }

        UVMapping(float scale_u, float scale_v)
        {
            this->scale_u = scale_u;
            this->scale_v = scale_v;
        }

        Eigen::Vector2f map(const HitInfo &rec) const override
        {
            return Eigen::Vector2f(rec.u * scale_u, rec.v * scale_v);
        }
    private:
        float scale_u, scale_v;
    };
}