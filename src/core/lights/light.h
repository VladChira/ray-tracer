#pragma once
#include "maths.h"
#include "ray.h"
#include "world.h"

namespace raytracer
{
    class Light
    {
    public:
        virtual Vector3 get_direction(const Ray &r_in, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi) = 0;
        virtual Color3 L(const Ray &r_in, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi) const = 0;
        virtual double G(const Ray &r_in, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi) const = 0;
        virtual double pdf(const Ray &r_in, const HitInfo &rec) const = 0;
        virtual bool in_shadow(const Ray &r, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi) const = 0;
    };
}