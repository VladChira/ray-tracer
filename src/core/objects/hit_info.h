#pragma once
#include <memory>

#include "maths.h"
#include "ray.h"
namespace raytracer
{
    class Material;
    class World;

    class HitInfo
    {
    public:
        raytracer::Point3 p;
        raytracer::Normal3 normal;
        double t;
        std::shared_ptr<Material> material;
        const World &world;

        HitInfo() = delete;

        HitInfo(const World &w) : world(w) {}

        void operator=(const HitInfo rec)
        {
            p = rec.p;
            normal = rec.normal;
            t = rec.t;
            material = rec.material;
        }

        bool front_face;

        inline void set_face_normal(const raytracer::Ray &r, const raytracer::Normal3 &outward_normal)
        {
            front_face = raytracer::Dot(r.direction, outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };
}