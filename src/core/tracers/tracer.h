#pragma once
#include "ray.h"
#include "sampler.h"
#include "world.h"

namespace raytracer
{
    class Tracer
    {
    public:
        virtual Color3 trace_ray(const raytracer::Ray &r, const World &world, int depth) const = 0;
    };
}