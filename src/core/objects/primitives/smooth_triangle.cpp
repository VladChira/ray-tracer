#include "smooth_triangle.h"
using namespace raytracer;

SmoothTriangle::SmoothTriangle()
{
    v0 = Point3(0, 0, 0);
    v1 = Point3(0, 0, 1);
    v2 = Point3(1, 0, 0);
    n0 = n1 = n2 = Normal3(0, 1, 0);
}

SmoothTriangle::SmoothTriangle(const Point3 &a, const Point3 &b, const Point3 &c, std::shared_ptr<Material> mat)
{
    v0 = a;
    v1 = b;
    v2 = c;
    this->material = mat;
}

bool SmoothTriangle::hit(const raytracer::Ray &ray, Interval t_range, HitInfo &rec) const
{
    float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.direction.x, d = v0.x - ray.origin.x;
    float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.direction.y, h = v0.y - ray.origin.y;
    float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.direction.z, l = v0.z - ray.origin.z;

    float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    float q = g * i - e * k, s = e * j - f * i;

    float inv_denom = 1.0f / (a * m + b * q + c * s);

    float e1 = d * m - b * n - c * p;
    float beta = e1 * inv_denom;

    if (beta < 0.0f) {
        return false;
    }

    float r = r = e * l - h * i;
    float e2 = a * n + d * q + c * r;
    float gamma = e2 * inv_denom;

    if (gamma < 0.0f) {
        return false;
    }

    if (beta + gamma > 1.0f) {
        return false;
    }

    float e3 = a * p - b * r + d * s;
    float t = e3 * inv_denom;

    if (t < std::numeric_limits<float>::epsilon()) {
        return false;
    }

    rec.t = t;
    rec.normal = interpolate_normal(beta, gamma);
    rec.p = ray.origin + t * ray.direction;
    rec.material = material;

    return true;
}

AABB SmoothTriangle::bounding_box() const
{
    const double delta = std::numeric_limits<double>::epsilon();
    Interval ix = Interval(std::min(std::min(v0.x, v1.x), v2.x) - delta, std::max(std::max(v0.x, v1.x), v2.x) + delta);
    Interval iy = Interval(std::min(std::min(v0.y, v1.y), v2.y) - delta, std::max(std::max(v0.y, v1.y), v2.y) + delta);
    Interval iz = Interval(std::min(std::min(v0.z, v1.z), v2.z) - delta, std::max(std::max(v0.z, v1.z), v2.z) + delta);

    return AABB(ix, iy, iz);
}

std::vector<std::shared_ptr<SmoothTriangle>> raytracer::tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps, std::shared_ptr<Material> mat)
{
    std::vector<std::shared_ptr<SmoothTriangle>> objects;
    int k = 1;

    for (int j = 0; j <= horizontal_steps - 1; j++)
    {
        // define vertices

        Point3 v0(0, 1, 0); // top

        Point3 v1(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // bottom left
                  cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

        Point3 v2(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // bottom  right
                  cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

        SmoothTriangle triangle_ptr(v0, v1, v2, mat);
        triangle_ptr.n0 = v0;
        triangle_ptr.n1 = v1;
        triangle_ptr.n2 = v2;
        objects.push_back(std::make_shared<SmoothTriangle>(triangle_ptr));
    }

    // define the bottom triangles

    k = vertical_steps - 1;

    for (int j = 0; j <= horizontal_steps - 1; j++)
    {
        // define vertices

        Point3 v0(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left
                  cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

        Point3 v1(0, -1, 0); // bottom

        Point3 v2(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // top right
                  cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

        SmoothTriangle triangle_ptr(v0, v1, v2, mat);
        triangle_ptr.n0 = v0;
        triangle_ptr.n1 = v1;
        triangle_ptr.n2 = v2;
        objects.push_back(std::make_shared<SmoothTriangle>(triangle_ptr));
    }

    //  define the other triangles

    for (int k = 1; k <= vertical_steps - 2; k++)
    {
        for (int j = 0; j <= horizontal_steps - 1; j++)
        {
            // define the first triangle

            // vertices

            Point3 v0(sin(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), // bottom left, use k + 1, j
                      cos(pi * (k + 1) / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps));

            Point3 v1(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), // bottom  right, use k + 1, j + 1
                      cos(pi * (k + 1) / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps));

            Point3 v2(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left,     use k, j
                      cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

            SmoothTriangle triangle_ptr1(v0, v1, v2, mat);
            triangle_ptr1.n0 = v0;
            triangle_ptr1.n1 = v1;
            triangle_ptr1.n2 = v2;
            objects.push_back(std::make_shared<SmoothTriangle>(triangle_ptr1));

            // define the second triangle

            // vertices

            v0 = Point3(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // top right, use k, j + 1
                        cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

            v1 = Point3(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left,     use k, j
                        cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

            v2 = Point3(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), // bottom  right, use k + 1, j + 1
                        cos(pi * (k + 1) / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps));

            SmoothTriangle triangle_ptr2(v0, v1, v2, mat);
            triangle_ptr2.n0 = v0;
            triangle_ptr2.n1 = v1;
            triangle_ptr2.n2 = v2;
            objects.push_back(std::make_shared<SmoothTriangle>(triangle_ptr2));
        }
    }
    return objects;
}

Normal3 SmoothTriangle::interpolate_normal(const float beta, const float gamma) const
{
    Normal3 normal((1 - beta - gamma) * n0 + beta * n1 + gamma * n2);
    normal = Normalize(normal);
    return normal;
}
