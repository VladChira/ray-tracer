#include "path_tracer.h"
#include "hit_info.h"
#include "material.h"

using namespace raytracer;

// Color3 PathTracer::trace_ray(const Ray &r, World &world, int depth)
// {
//     // ZoneScoped;
//     if (depth <= 0)
//         return Color3(0, 0, 0);

//     HitInfo rec(world);
//     if (world.hit_objects(r, Interval(0.0001, infinity), rec))
//     {
//         Color3 total(0, 0, 0);

//         total += rec.material->emitted(r, rec);
        

//         Ray scattered;
//         Color3 attenuation;
//         if (rec.material != nullptr && rec.material->scatter(r, rec, attenuation, scattered))
//         {
//             total += (attenuation * trace_ray(scattered, world, depth - 1));
//         }
//         // std::cout << total << "\n";
//         return total;
//     }
//     return Color3::black;

//     // blue sky color
//     // Vector3 unit_direction = Normalize(r.direction);
//     // auto t = 0.5 * (unit_direction.y + 1.0);
//     // return ((1.0 - t) * Color3::white + t * Color3(0.4, 0.6, 0.9));

//     // Vector3 unit_direction = Normalize(r.direction);
//     // auto t = 0.5 * (unit_direction.y + 1.0);
//     // return ((1.0 - t) * Color3::black + t * Color3(0.08, 0.08, 0.08));
// }

Color3 PathTracer::trace_ray(const raytracer::Ray &r, World &world, int depth)
{
    // std::cout << depth << "\n";
    if (depth <= 0)
        return Color3::black;
    HitInfo rec(world);
    if (world.hit_objects(r, Interval(0.0001, infinity), rec))
    {
        rec.depth = depth;
        return rec.material->path_shade(r, rec);
    }
    // return Color3::black;

    Vector3 unit_direction = Normalize(r.direction);
    auto t = 0.5 * (unit_direction.y + 1.0);
    return ((1.0 - t) * Color3::white + t * Color3(0.4, 0.6, 0.9));
}
