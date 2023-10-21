#include "path_tracer.h"
#include "hit_info.h"
#include "material.h"

using namespace raytracer;

// Color PathTracer::trace_ray(const Ray &r, World &world, int depth)
// {
//     Color L(0.0, 0.0, 0.0);
//     Ray ray;

//     for (int bounce = 0; bounce < depth; bounce++)
//     {
//         // Find next path vertex and accumulate contribution
//         // Intersect ray with scene
//         HitInfo rec(world);
//         bool hit = world.hit_objects(r, Interval(0.0001, infinity), rec);
//     }
// }

Color PathTracer::trace_ray(const Ray &r, World &world, int depth)
{
    // ZoneScoped;
    if (depth <= 0)
        return Color(0, 0, 0);

    HitInfo rec(world);
    if (world.hit_objects(r, Interval(0.0001, infinity), rec))
    {
        Color total(0, 0, 0);

        total += rec.material->emitted(r, rec);

        Ray scattered;
        Color attenuation;
        if (rec.material != nullptr && rec.material->scatter(r, rec, attenuation, scattered))
        {
            total += (attenuation * trace_ray(scattered, world, depth - 1));
        }
        return total;
    }

    if (world.hdri != nullptr)
    {
        Eigen::Vector3f unit_direction = r.direction.normalized();
        unit_direction = Transform::RotateY(-10).transform_normal(unit_direction);
        float theta = acos(-unit_direction.y());
        float phi = atan2(-unit_direction.z(), unit_direction.x()) + pi;
        float u = phi / (2 * pi);
        float v = theta / pi;
        HitInfo rec2(world);
        rec2.u = u;
        rec2.v = v;
        return world.hdri->evaluate(rec2);
    }
    else
    {
        // return Color::black;

        // blue sky color
        Eigen::Vector3f unit_direction = r.direction.normalized();
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (Color::white * (1.0 - t)  + Color(0.4, 0.6, 0.9) * t);
    }

    // Vector3 unit_direction = Normalize(r.direction);
    // auto t = 0.5 * (unit_direction.y + 1.0);
    // return ((1.0 - t) * Color::black + t * Color(0.08, 0.08, 0.08));
}

// Color PathTracer::trace_ray(const raytracer::Ray &r, World &world, int depth)
// {
//     // std::cout << depth << "\n";
//     if (depth <= 0)
//         return Color::black;
//     HitInfo rec(world);
//     if (world.hit_objects(r, Interval(0.0001, infinity), rec))
//     {
//         rec.depth = depth;
//         return rec.material->path_shade(r, rec);
//     }
//     return Color::black;

//     // Eigen::Vector3f unit_direction = r.direction.normalized();
//     // auto t = 0.5 * (unit_direction.y() + 1.0);
//     // return (Color::white * (1.0 - t) + Color(0.4, 0.6, 0.9) * t);
// }
