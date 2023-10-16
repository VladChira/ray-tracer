#pragma once

#include "texture.h"
#include "utilities.h"

namespace raytracer
{
    class ConstantTexture : public Texture
    {
    public:
        ConstantTexture(const Color &c)
        {
            this->c = c;
        }
        Color evaluate(const HitInfo &rec) const override
        {
            return c;
        }
    private:
        Color c;
    };
}