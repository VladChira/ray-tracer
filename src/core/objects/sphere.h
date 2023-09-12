#pragma once
#include "geometric_object.h"
#include "maths.h"

namespace raytracer
{
    class Sphere : public GeometricObject
    {
    public:
        raytracer::Point3 center;
        double radius;
        std::shared_ptr<raytracer::Material> material;

        Sphere()
        {
            center = Vector3(0, 0, 0);
            radius = 0;
        }

        Sphere(const raytracer::Vector3 &c, double r, std::shared_ptr<raytracer::Material> mat)
        {
            this->center = c;
            this->radius = r;
            this->material = mat;
        }

        bool hit(const raytracer::Ray &r, double t_min, double t_max, HitInfo &rec) const
        {
            raytracer::Vector3 oc = r.origin - center;
            auto a = r.direction.LengthSquared();
            auto half_b = raytracer::Dot(oc, r.direction);
            auto c = oc.LengthSquared() - radius * radius;

            auto discriminant = half_b * half_b - a * c;
            if (discriminant < 0)
                return false;
            auto sqrtd = sqrt(discriminant);

            auto root = (-half_b - sqrtd) / a;
            if (root < t_min || t_max < root)
            {
                root = (-half_b + sqrtd) / a;
                if (root < t_min || t_max < root)
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.material = this->material;

            raytracer::Normal3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            return true;
        }
    };

}