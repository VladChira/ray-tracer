#include "sphere.h"

using namespace raytracer;

Sphere::Sphere()
{
    center = Eigen::Vector3f(0, 0, 0);
    radius = 1;
    material = nullptr;
    auto rvec = Eigen::Vector3f(radius, radius, radius);
    aabb = Eigen::AlignedBox3f(center - rvec, center + rvec);
}

Sphere::Sphere(const Eigen::Vector3f &c, float r, std::shared_ptr<raytracer::Material> mat)
{
    this->center = c;
    this->radius = r;
    this->material = mat;
    auto rvec = Eigen::Vector3f(radius, radius, radius);
    aabb = Eigen::AlignedBox3f(center - rvec, center + rvec);
}

bool Sphere::hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const
{
    if (r.is_camera_ray && !this->visible_to_camera) return false;
    
    float t_min = t_range.min;
    float t_max = t_range.max;
    Eigen::Vector3f oc = r.origin - center;
    auto a = r.direction.squaredNorm();
    auto half_b = oc.dot(r.direction);
    auto c = oc.squaredNorm() - radius * radius;

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

    Eigen::Vector3f outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    return true;
}

Eigen::AlignedBox3f Sphere::bounding_box() const
{
    return aabb;
}