#include "ray_caster.h"

using namespace raytracer;

Color3 RayCaster::trace_ray(const raytracer::Ray &r, const raytracer::World &world, int depth) const
{
    if (depth <= 0)
        return raytracer::Color3(0, 0, 0);
    HitInfo rec;
    rec.lights = world.lights;
    if (world.objects.hit(r, Interval(0.001, infinity), rec))
    {
        return rec.material->shade(r, rec);
    }
    return Color3(0, 0, 0);
}