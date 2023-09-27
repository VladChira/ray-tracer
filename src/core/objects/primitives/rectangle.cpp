#include "rectangle.h"
#include "multijittered.h"
using namespace raytracer;

Rectangle::Rectangle(const Point3 &p0, const Vector3 &a, const Vector3 &b, std::shared_ptr<raytracer::Material> mat)
{
    this->p0 = p0;
    this->a = a;
    this->b = b;
    this->normal = Normalize(Cross(a, b));
    this->material = mat;
    sampler = std::make_shared<MultiJittered>(300);
    this->a_len_sq = a.LengthSquared();
    this->b_len_sq = b.LengthSquared();
    this->area = a.Length() * b.Length();
    this->inv_area = 1 / this->area;

    const double delta = 0.00001;
    Interval ix = Interval(std::min(p0.x, p0.x + a.x + b.x) - delta, std::max(p0.x, p0.x + a.x + b.x) + delta);
    Interval iy = Interval(std::min(p0.y, p0.y + a.y + b.y) - delta, std::max(p0.y, p0.y + a.y + b.y) + delta);
    Interval iz = Interval(std::min(p0.z, p0.z + a.z + b.z) - delta, std::max(p0.z, p0.z + a.z + b.z) + delta);
    aabb = AABB(ix, iy, iz);
}

Rectangle::Rectangle(const Point3 &p0, const Vector3 &a, const Vector3 &b, const Normal3 normal, std::shared_ptr<raytracer::Material> mat)
{
    this->p0 = p0;
    this->a = a;
    this->b = b;
    this->normal = Normalize(normal);
    this->material = mat;
    sampler = std::make_shared<MultiJittered>(300);
    this->a_len_sq = a.LengthSquared();
    this->b_len_sq = b.LengthSquared();
    this->area = a.Length() * b.Length();
    this->inv_area = 1 / this->area;

    const double delta = 0.00001;
    Interval ix = Interval(std::min(p0.x, p0.x + a.x + b.x) - delta, std::max(p0.x, p0.x + a.x + b.x) + delta);
    Interval iy = Interval(std::min(p0.y, p0.y + a.y + b.y) - delta, std::max(p0.y, p0.y + a.y + b.y) + delta);
    Interval iz = Interval(std::min(p0.z, p0.z + a.z + b.z) - delta, std::max(p0.z, p0.z + a.z + b.z) + delta);
    aabb = AABB(ix, iy, iz);
}

bool Rectangle::hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const
{
    double t = Dot((p0 - r.origin), normal) / Dot(r.direction, normal);

    if (t <= 0.0001)
    {
        return false;
    }

    Point3 p = r.origin + t * r.direction;
    Vector3 d = p - p0;

    double ddota = Dot(d, a);

    if (ddota < 0.0f || ddota > a_len_sq)
    {
        return false;
    }

    double ddotb = Dot(d, b);

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

Point3 Rectangle::sample() const
{
    Point2 sample_point = Point2(random_double(), random_double()); //sampler->sample_unit_square();
    return p0 + sample_point.x * a + sample_point.y * b;
}

double Rectangle::pdf(const raytracer::Ray &r, const HitInfo &rec) const
{
    return inv_area;
}

AABB Rectangle::bounding_box() const
{
    return aabb;
}

Point3 Rectangle::get_normal(const Point3 p) const
{
    return normal;
}