#pragma once
#include "tracer.h"
namespace raytracer
{
    class PathTracer : public Tracer
    {
    public:
        PathTracer() {}
        Color trace_ray(const raytracer::Ray &r, World &world, int depth) override;
    };
}