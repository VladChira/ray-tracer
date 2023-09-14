#pragma once
#include "ray.h"
#include "hittable_list.h"
#include "sampler.h"

namespace raytracer
{
    class Tracer
    {
    public:
        virtual Color3 trace_ray(const raytracer::Ray &r, const raytracer::HittableList &world, int depth) const = 0;
    };
}