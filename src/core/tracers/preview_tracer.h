#pragma once
#include "tracer.h"
namespace raytracer
{
    class PreviewTracer : public Tracer
    {
    public:
        PreviewTracer() {}
        Color trace_ray(const raytracer::Ray &r, World &world, int depth) override;
    };
}