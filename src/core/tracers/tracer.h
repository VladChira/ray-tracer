#pragma once
#include "ray.h"
#include "sampler.h"
#include "world.h"

namespace raytracer
{
    class Tracer
    {
    public:
        virtual Color3 trace_ray(const raytracer::Ray &r, World &world, int depth) = 0;
    };
}