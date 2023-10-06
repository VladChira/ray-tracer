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
    if (r.is_camera_ray && !this->visible_to_camera)
        return false;

    bool transformed = this->transform != nullptr;

    Ray tr;
    tr.origin = r.origin;
    tr.direction = r.direction;
    if (transformed)
    {
        tr.origin = Transform::Inverse(*transform).transform_point(r.origin);
        tr.direction = Transform::Inverse(*transform).transform_vector(r.direction);
    }

    float t_min = t_range.min;
    float t_max = t_range.max;
    Eigen::Vector3f oc = tr.origin - center;
    auto a = tr.direction.squaredNorm();
    auto half_b = oc.dot(tr.direction);
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
    rec.p = tr.at(rec.t);
    rec.normal = (rec.p - center) / radius;
    rec.material = this->material;

    Eigen::Vector3f outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(tr, outward_normal);

    if (transformed)
    {
        rec.p = transform->transform_point(rec.p);
        rec.normal = transform->transform_normal(rec.normal);
    }
    return true;
}

Eigen::AlignedBox3f Sphere::bounding_box() const
{
    if (this->transform != nullptr)
        return this->transform->transform_bounding_box(aabb);
    return aabb;
}