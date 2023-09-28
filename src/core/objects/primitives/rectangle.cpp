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

    if (t < t_range.min || t > t_range.max)
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
    Point2 sample_point = Point2(random_double(), random_double()); // sampler->sample_unit_square();
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

std::vector<std::shared_ptr<GeometricObject>> raytracer::create_box(const Point3 &a, const Point3 &b, std::shared_ptr<Material> mat)
{
    std::vector<std::shared_ptr<GeometricObject>> sides;
    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = Point3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    auto max = Point3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    auto dx = Vector3(max.x - min.x, 0, 0);
    auto dy = Vector3(0, max.y - min.y, 0);
    auto dz = Vector3(0, 0, max.z - min.z);

    sides.push_back(std::make_shared<Rectangle>(Point3(min.x, min.y, max.z),  dx,  dy, mat)); // front
    sides.push_back(std::make_shared<Rectangle>(Point3(max.x, min.y, max.z), -dz,  dy, mat)); // right
    sides.push_back(std::make_shared<Rectangle>(Point3(max.x, min.y, min.z), -dx,  dy, mat)); // back
    sides.push_back(std::make_shared<Rectangle>(Point3(min.x, min.y, min.z),  dz,  dy, mat)); // left
    sides.push_back(std::make_shared<Rectangle>(Point3(min.x, max.y, max.z),  dx, -dz, mat)); // top
    sides.push_back(std::make_shared<Rectangle>(Point3(min.x, min.y, min.z),  dx,  dz, mat)); // bottom

    // sides.push_back(std::make_shared<Rectangle>(Point3(min.x, min.y, max.z),  dy,  dx, mat)); // front
    // sides.push_back(std::make_shared<Rectangle>(Point3(max.x, min.y, max.z),  dy, -dz, mat)); // right
    // sides.push_back(std::make_shared<Rectangle>(Point3(max.x, min.y, min.z),  dy, -dx, mat)); // back
    // sides.push_back(std::make_shared<Rectangle>(Point3(min.x, min.y, min.z),  dy,  dz, mat)); // left
    // sides.push_back(std::make_shared<Rectangle>(Point3(min.x, max.y, max.z),  dx, -dz, mat)); // top
    // sides.push_back(std::make_shared<Rectangle>(Point3(min.x, min.y, min.z),  dx,  dz, mat)); // bottom

    return sides;
}