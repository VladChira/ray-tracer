#include "path_tracer.h"
using namespace raytracer;

Color3 PathTracer::trace_ray(const raytracer::Ray &r, const raytracer::HittableList &world, int depth) const
{
    if (depth <= 0)
        return raytracer::Color3(0, 0, 0);

    HitInfo rec;
    if (world.hit(r, 0.001, infinity, rec))
    {
        raytracer::Ray scattered;
        raytracer::Color3 attenuation;
        if (rec.material != NULL && rec.material->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * trace_ray(scattered, world, depth - 1);
        }
        return raytracer::Color3(0, 0, 0);
    }
    raytracer::Vector3 unit_direction = raytracer::Normalize(r.direction);
    auto t = 0.5 * (unit_direction.y + 1.0);
    return ((1.0 - t) * raytracer::Color3(1.0, 1.0, 1.0) + t * raytracer::Color3(0.5, 0.7, 1.0));
}