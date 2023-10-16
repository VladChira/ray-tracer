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

    aabb = compute_aabb(p0, a, b);
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

    aabb = compute_aabb(p0, a, b);
}

bool Rectangle::hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const
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

    float t = (p0 - tr.origin).dot(normal) / tr.direction.dot(normal);

    if (t < t_range.min || t > t_range.max)
    {
        return false;
    }

    Eigen::Vector3f p = tr.origin + t * tr.direction;
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
    // rec.local_p.x() = ddota;
    // rec.local_p.y() = ddotb;
    rec.material = material;

    if (transformed)
    {
        rec.p = transform->transform_point(rec.p);
        rec.normal = transform->transform_normal(rec.normal);
    }

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

float Rectangle::pdf_solid_angle(const HitInfo &rec, const Eigen::Vector3f &wi) const
{
    // Intersect a ray with rectangle geometry
    Ray r(rec.p, wi);
    float t;
    HitInfo rec2(rec.world);
    if (!hit(r, Interval(0, infinity), rec2))
        return 0;

    // Convert from area to solid angle
    float pdf = (rec.p - rec2.p).squaredNorm() / (fabs(rec2.normal.dot(-wi)) * area);
    return pdf;
}

Eigen::AlignedBox3f Rectangle::bounding_box() const
{
    if (this->transform != nullptr)
        return this->transform->transform_bounding_box(aabb);
    return aabb;
}

Eigen::Vector3f Rectangle::get_normal(const Eigen::Vector3f p) const
{
    return normal;
}

std::vector<std::shared_ptr<GeometricObject>> raytracer::create_box(float width, float height, float depth, std::shared_ptr<Material> mat, Transform *t)
{
    float half_extent_x = width / 2;
    float half_extent_y = height / 2;
    float half_extent_z = depth / 2;

    Eigen::Vector3f a(-half_extent_x, -half_extent_y, -half_extent_z);
    Eigen::Vector3f b(half_extent_x, half_extent_y, half_extent_z);

    std::vector<std::shared_ptr<GeometricObject>> sides;
    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = Eigen::Vector3f(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
    auto max = Eigen::Vector3f(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

    auto dx = Eigen::Vector3f(max.x() - min.x(), 0, 0);
    auto dy = Eigen::Vector3f(0, max.y() - min.y(), 0);
    auto dz = Eigen::Vector3f(0, 0, max.z() - min.z());

    auto front = std::make_shared<Rectangle>(Eigen::Vector3f(min.x(), min.y(), max.z()), dx, dy, mat);
    front->set_transform(t);

    auto right = std::make_shared<Rectangle>(Eigen::Vector3f(max.x(), min.y(), max.z()), -dz, dy, mat);
    right->set_transform(t);

    auto back = std::make_shared<Rectangle>(Eigen::Vector3f(max.x(), min.y(), min.z()), -dx, dy, mat);
    back->set_transform(t);

    auto left = std::make_shared<Rectangle>(Eigen::Vector3f(min.x(), min.y(), min.z()), dz, dy, mat);
    left->set_transform(t);

    auto top = std::make_shared<Rectangle>(Eigen::Vector3f(min.x(), max.y(), max.z()), dx, -dz, mat);
    top->set_transform(t);

    auto bottom = std::make_shared<Rectangle>(Eigen::Vector3f(min.x(), min.y(), min.z()), dx, dz, mat);
    bottom->set_transform(t);

    sides.push_back(front);
    sides.push_back(right);
    sides.push_back(back);
    sides.push_back(left);
    sides.push_back(top);
    sides.push_back(bottom);

    return sides;
}

static Eigen::AlignedBox3f compute_aabb(Eigen::Vector3f p0, Eigen::Vector3f a, Eigen::Vector3f b)
{
    Eigen::AlignedBox3f bbox;
    bbox.extend(p0);
    bbox.extend(p0 + a);
    bbox.extend(p0 + b);
    bbox.extend(p0 + a + b);

    // Displace the corners by a tiny amount to avoid degenerate bounding boxes
    bbox.min() -= Eigen::Vector3f(0.0001, 0.0001, 0.0001);
    bbox.max() += Eigen::Vector3f(0.0001, 0.0001, 0.0001);

    return bbox;
}