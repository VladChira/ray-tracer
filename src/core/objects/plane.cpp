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

bool Plane::hit(const raytracer::Ray &r, double t_min, double t_max, HitInfo &rec) const
{
    double numerator = Dot(a - r.origin, this->n);
    double denominator = Dot(r.direction, this->n);
    
    double t = numerator / denominator;
    if (t > 0.0001) {
        // printf("%f\n", t);
        rec.t = t;
        rec.p = r.origin + r.direction * t;
        rec.normal = -this->n;
        rec.material = this->material;
        //rec.material = NULL;
        return true;
    }

    return false;
}