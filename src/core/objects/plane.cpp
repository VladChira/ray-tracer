#include "plane.h"
using namespace raytracer;

Plane::Plane()
{
    this->a = Vector3(0, 0, 0);
    this->n = Normal3(0, 1, 0);
    this->material = NULL;
}

Plane::Plane(raytracer::Point3 point, raytracer::Normal3 n)
{
    this->a = point;
    this->n = Normalize(n);
    this->material = NULL;
}

Plane::Plane(raytracer::Point3 point, raytracer::Normal3 n, std::shared_ptr<raytracer::Material> mat)
{
    this->a = point;
    this->n = Normalize(n);
    this->material = mat;
}

bool Plane::hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const
{
    double t = Dot(a - r.origin, this->n) / Dot(r.direction, this->n);
    if (t < t_range.min || t > t_range.max) return false;
    if (t > 0.0001) {
        rec.t = t;
        rec.p = r.origin + r.direction * t;
        rec.normal = this->n;
        rec.material = this->material;
        return true;
    }
    return false;
}