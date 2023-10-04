#pragma once
#include "tracer.h"
namespace raytracer
{
    class RayTracer : public Tracer
    {
    public:
        RayTracer() {}
        Color trace_ray(const raytracer::Ray &r, World &world, int depth) override;
    };
}