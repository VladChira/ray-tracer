#include "triangle.h"
using namespace raytracer;

Triangle::Triangle()
{
    v0 = Eigen::Vector3f(0, 0, 0);
    v1 = Eigen::Vector3f(0, 0, 1);
    v2 = Eigen::Vector3f(1, 0, 0);
    normal = Eigen::Vector3f(0, 1, 0);
}

Triangle::Triangle(const Eigen::Vector3f &a, const Eigen::Vector3f &b, const Eigen::Vector3f &c, std::shared_ptr<Material> mat)
{
    v0 = a;
    v1 = b;
    v2 = c;
    normal = ((v1 - v0).cross(v2 - v0)).normalized();
    this->material = mat;

    const float delta = 0.00001;
    Interval ix = Interval(std::min(std::min(v0.x(), v1.x()), v2.x()) - delta, std::max(std::max(v0.x(), v1.x()), v2.x()) + delta);
    Interval iy = Interval(std::min(std::min(v0.y(), v1.y()), v2.y()) - delta, std::max(std::max(v0.y(), v1.y()), v2.y()) + delta);
    Interval iz = Interval(std::min(std::min(v0.z(), v1.z()), v2.z()) - delta, std::max(std::max(v0.z(), v1.z()), v2.z()) + delta);

    aabb = AABB(ix, iy, iz);
}

bool Triangle::hit(const raytracer::Ray &ray, Interval t_range, HitInfo &rec) const
{
    float a = v0.x() - v1.x(), b = v0.x() - v2.x(), c = ray.direction.x(), d = v0.x() - ray.origin.x();
    float e = v0.y() - v1.y(), f = v0.y() - v2.y(), g = ray.direction.y(), h = v0.y() - ray.origin.y();
    float i = v0.z() - v1.z(), j = v0.z() - v2.z(), k = ray.direction.z(), l = v0.z() - ray.origin.z();

    float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    float q = g * i - e * k, s = e * j - f * i;

    float inv_denom = 1.0 / (a * m + b * q + c * s);

    float e1 = d * m - b * n - c * p;
    float beta = e1 * inv_denom;

    if (beta < 0.0)
    {
        return false;
    }

    float r = e * l - h * i;
    float e2 = a * n + d * q + c * r;
    float gamma = e2 * inv_denom;

    if (gamma < 0.0)
    {
        return false;
    }

    if (beta + gamma > 1.0)
    {
        return false;
    }

    float e3 = a * p - b * r + d * s;
    float t = e3 * inv_denom;

    if (t < std::numeric_limits<float>::epsilon())
    {
        return false;
    }

    rec.t = t;
    rec.normal = normal;
    rec.p = ray.origin + t * ray.direction;
    rec.material = this->material;

    return true;
}

AABB Triangle::bounding_box() const
{
    return aabb;
}