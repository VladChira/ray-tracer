#include "path_tracer.h"
#include "hit_info.h"
#include "material.h"

using namespace raytracer;

// Color PathTracer::trace_ray(const Ray &r, World &world, int depth)
// {
//     // ZoneScoped;
//     if (depth <= 0)
//         return Color(0, 0, 0);

//     HitInfo rec(world);
//     if (world.hit_objects(r, Interval(0.0001, infinity), rec))
//     {
//         Color total(0, 0, 0);

//         total += rec.material->emitted(r, rec);
        

//         Ray scattered;
//         Color attenuation;
//         if (rec.material != nullptr && rec.material->scatter(r, rec, attenuation, scattered))
//         {
//             total += (attenuation * trace_ray(scattered, world, depth - 1));
//         }
//         // std::cout << total << "\n";
//         return total;
//     }
//     return Color::black;

//     // blue sky color
//     // Eigen::Vector3f unit_direction = r.direction.normalized();
//     // auto t = 0.5 * (unit_direction.y() + 1.0);
//     // return (Color::white * (1.0 - t)  + Color(0.4, 0.6, 0.9) * t);

//     // Vector3 unit_direction = Normalize(r.direction);
//     // auto t = 0.5 * (unit_direction.y + 1.0);
//     // return ((1.0 - t) * Color::black + t * Color(0.08, 0.08, 0.08));
// }

Color PathTracer::trace_ray(const raytracer::Ray &r, World &world, int depth)
{
    // std::cout << depth << "\n";
    if (depth <= 0)
        return Color::black;
    HitInfo rec(world);
    if (world.hit_objects(r, Interval(0.0001, infinity), rec))
    {
        rec.depth = depth;
        return rec.material->path_shade(r, rec);
    }
    return Color::black;

    // Eigen::Vector3f unit_direction = r.direction.normalized();
    // auto t = 0.5 * (unit_direction.y() + 1.0);
    // return (Color::white * (1.0 - t) + Color(0.4, 0.6, 0.9) * t);
}
