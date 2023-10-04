#pragma once
#include "ray.h"
#include "sampler.h"
#include "world.h"
#include "utilities.h"

namespace raytracer
{
    class Tracer
    {
    public:
        virtual Color trace_ray(const raytracer::Ray &r, World &world, int depth) = 0;
    };
}