#pragma once
#include "tracer.h"
namespace raytracer
{
    class DirectLightingTracer : public Tracer
    {
    public:
        DirectLightingTracer() {}
        Color trace_ray(const raytracer::Ray &r, World &world, int depth) override;
    };
}