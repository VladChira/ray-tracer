#pragma once
#include "tracer.h"
namespace raytracer
{
    class RayCaster : public Tracer
    {
    public:
        RayCaster() {}
        Color3 trace_ray(const raytracer::Ray &r, const World &world, int depth) const override;
    };
}