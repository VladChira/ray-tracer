#pragma once
#include "tracer.h"
namespace raytracer
{
    class PathTracer : public Tracer
    {
    public:
        PathTracer() {}
        Color3 trace_ray(const raytracer::Ray &r, const World &world, int depth) const override;
    };
}