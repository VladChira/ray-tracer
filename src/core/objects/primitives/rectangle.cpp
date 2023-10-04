#include "rectangle.h"
#include "multijittered.h"
using namespace raytracer;

Rectangle::Rectangle(const Eigen::Vector3f &p0, const Eigen::Vector3f &a, const Eigen::Vector3f &b, std::shared_ptr<raytracer::Material> mat)
{
    this->p0 = p0;
    this->a = a;
    this->b = b;
    this->normal = (a.cross(b)).normalized();
    this->material = mat;
    sampler = std::make_shared<MultiJittered>(20);
    this->a_len_sq = a.squaredNorm();
    this->b_len_sq = b.squaredNorm();
    this->area = a.norm() * b.norm();
    this->inv_area = 1 / this->area;

    const float delta = 0.0001;
    Interval ix = Interval(std::min(p0.x(), p0.x() + a.x() + b.x()) - delta, std::max(p0.x(), p0.x() + a.x() + b.x()) + delta);
    Interval iy = Interval(std::min(p0.y(), p0.y() + a.y() + b.y()) - delta, std::max(p0.y(), p0.y() + a.y() + b.y()) + delta);
    Interval iz = Interval(std::min(p0.z(), p0.z() + a.z() + b.z()) - delta, std::max(p0.z(), p0.z() + a.z() + b.z()) + delta);
    aabb = AABB(ix, iy, iz);
}

Rectangle::Rectangle(const Eigen::Vector3f &p0, const Eigen::Vector3f &a, const Eigen::Vector3f &b, const Eigen::Vector3f normal, std::shared_ptr<raytracer::Material> mat)
{
    this->p0 = p0;
    this->a = a;
    this->b = b;
    this->normal = normal.normalized();
    this->material = mat;
    sampler = std::make_shared<MultiJittered>(20);
    this->a_len_sq = a.squaredNorm();
    this->b_len_sq = b.squaredNorm();
    this->area = a.norm() * b.norm();
    this->inv_area = 1 / this->area;

    const float delta = 0.0001;
    Interval ix = Interval(std::min(p0.x(), p0.x() + a.x() + b.x()) - delta, std::max(p0.x(), p0.x() + a.x() + b.x()) + delta);
    Interval iy = Interval(std::min(p0.y(), p0.y() + a.y() + b.y()) - delta, std::max(p0.y(), p0.y() + a.y() + b.y()) + delta);
    Interval iz = Interval(std::min(p0.z(), p0.z() + a.z() + b.z()) - delta, std::max(p0.z(), p0.z() + a.z() + b.z()) + delta);
    aabb = AABB(ix, iy, iz);
}

bool Rectangle::hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const
{
    float t = (p0 - r.origin).dot(normal) / r.direction.dot(normal);

    if (t < t_range.min || t > t_range.max)
    {
        return false;
    }

    Eigen::Vector3f p = r.origin + t * r.direction;
    Eigen::Vector3f d = p - p0;

    float ddota = d.dot(a);

    if (ddota < 0.0f || ddota > a_len_sq)
    {
        return false;
    }

    float ddotb = d.dot(b);

    if (ddotb < 0.0f || ddotb > b_len_sq)
    {
        return false;
    }

    rec.t = t;
    rec.normal = normal;
    rec.p = p;
    rec.material = material;

    return true;
}

Eigen::Vector3f Rectangle::sample() const
{
    Eigen::Vector2f sample_point = Eigen::Vector2f(random_float(), random_float()); // sampler->sample_unit_square();
    return p0 + sample_point.x() * a + sample_point.y() * b;
}

float Rectangle::pdf(const raytracer::Ray &r, const HitInfo &rec) const
{
    return inv_area;
}

AABB Rectangle::bounding_box() const
{
    return aabb;
}

Eigen::Vector3f Rectangle::get_normal(const Eigen::Vector3f p) const
{
    return normal;
}

std::vector<std::shared_ptr<GeometricObject>> raytracer::create_box(const Eigen::Vector3f &a, const Eigen::Vector3f &b, std::shared_ptr<Material> mat)
{
    std::vector<std::shared_ptr<GeometricObject>> sides;
    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = Eigen::Vector3f(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
    auto max = Eigen::Vector3f(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

    auto dx = Eigen::Vector3f(max.x() - min.x(), 0, 0);
    auto dy = Eigen::Vector3f(0, max.y() - min.y(), 0);
    auto dz = Eigen::Vector3f(0, 0, max.z() - min.z());

    sides.push_back(std::make_shared<Rectangle>(Eigen::Vector3f(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides.push_back(std::make_shared<Rectangle>(Eigen::Vector3f(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides.push_back(std::make_shared<Rectangle>(Eigen::Vector3f(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides.push_back(std::make_shared<Rectangle>(Eigen::Vector3f(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides.push_back(std::make_shared<Rectangle>(Eigen::Vector3f(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides.push_back(std::make_shared<Rectangle>(Eigen::Vector3f(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
}