#include "ray_caster.h"
using namespace raytracer;

Color3 RayCaster::trace_ray(const raytracer::Ray &r, const raytracer::HittableList &world, int depth) const
{
    if (depth <= 0)
        return raytracer::Color3(0, 0, 0);
    HitInfo rec;
    if (world.hit(r, 0.001, infinity, rec))
    {
        raytracer::Ray scattered;
        raytracer::Color3 attenuation;
        return raytracer::Color3(1, 1, 1);
    }
    return Color3(0, 0, 0);
}