#include "path_tracer.h"
using namespace raytracer;

Color3 PathTracer::trace_ray(const raytracer::Ray &r, const raytracer::World &world, int depth) const
{
    if (depth <= 0)
        return raytracer::Color3(0, 0, 0);

    HitInfo rec;
    if (world.objects.hit(r, Interval(0.0001, infinity), rec))
    {
        raytracer::Ray scattered;
        raytracer::Color3 attenuation;
        if (rec.material != nullptr && rec.material->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * trace_ray(scattered, world, depth - 1);
        }
        return raytracer::Color3::black;
    }
    // return Color3(0.8, 0.8, 0.8);
    raytracer::Vector3 unit_direction = raytracer::Normalize(r.direction);
    auto t = 0.5 * (unit_direction.y + 1.0);
    return ((1.0 - t) * raytracer::Color3::white + t * raytracer::Color3(0.4, 0.6, 0.9));
}