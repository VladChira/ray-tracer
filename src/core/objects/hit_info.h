#pragma once
#include <memory>

#include "utilities.h"
#include "ray.h"
namespace raytracer
{
    class Material;
    class World;

    class HitInfo
    {
    public:
        Eigen::Vector3f p;
        Eigen::Vector3f normal;
        float t;
        std::shared_ptr<Material> material;
        World &world;
        int depth = 0;

        HitInfo() = delete;

        HitInfo(World &w) : world(w) {}

        void operator=(HitInfo &rec)
        {
            p = rec.p;
            normal = rec.normal;
            t = rec.t;
            material = rec.material;
        }

        bool front_face;

        inline void set_face_normal(const raytracer::Ray &r, const Eigen::Vector3f &outward_normal)
        {
            front_face = r.direction.dot(outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };
}