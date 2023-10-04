#include "preview_tracer.h"
#include "material.h"

using namespace raytracer;

Color PreviewTracer::trace_ray(const raytracer::Ray &r, World &world, int depth)
{
    if (depth <= 0)
        return Color::black;
    HitInfo rec(world);
    if (world.hit_objects(r, Interval(0.001, infinity), rec))
    {
        return rec.material->preview_shade(r, rec);
    }
    return Color::black;
}