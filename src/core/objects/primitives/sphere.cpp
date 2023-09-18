#include "sphere.h"
#include "aabb.h"
using namespace raytracer;

Sphere::Sphere()
{
    center = Vector3(0, 0, 0);
    radius = 1;
    material = nullptr;
    auto rvec = Vector3(radius, radius, radius);
    aabb = AABB(center - rvec, center + rvec);
}

Sphere::Sphere(const raytracer::Vector3 &c, double r, std::shared_ptr<raytracer::Material> mat)
{
    this->center = c;
    this->radius = r;
    this->material = mat;
    auto rvec = Vector3(radius, radius, radius);
    aabb = AABB(center - rvec, center + rvec);
}

bool Sphere::hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const
{
    double t_min = t_range.min;
    double t_max = t_range.max;
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

AABB Sphere::bounding_box() const
{
    return aabb;
}