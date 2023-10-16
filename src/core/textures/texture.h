#pragma once

#include "hit_info.h"
#include "utilities.h"

namespace raytracer
{
    class Texture
    {
    public:
        virtual Color evaluate(const HitInfo &rec) const = 0;
        virtual ~Texture() {}
    };
}
