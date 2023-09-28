#pragma once
#include "tracer.h"
namespace raytracer
{
    class RayTracer : public Tracer
    {
    public:
        RayTracer() {}
        Color3 trace_ray(const raytracer::Ray &r, World &world, int depth) override;
    };
}