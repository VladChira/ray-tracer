#include "triangle.h"
using namespace raytracer;

Triangle::Triangle()
{
    v0 = Point3(0, 0, 0);
    v1 = Point3(0, 0, 1);
    v2 = Point3(1, 0, 0);
    normal = Normal3(0, 1, 0);
}

Triangle::Triangle(const Point3 &a, const Point3 &b, const Point3 &c, std::shared_ptr<Material> mat)
{
    v0 = a;
    v1 = b;
    v2 = c;
    normal = Normalize(Cross(v1 - v0, v2 - v0));
    this->material = mat;
}

bool Triangle::hit(const raytracer::Ray &ray, Interval t_range, HitInfo &rec) const
{
    double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.direction.x, d = v0.x - ray.origin.x;
    double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.direction.y, h = v0.y - ray.origin.y;
    double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.direction.z, l = v0.z - ray.origin.z;

    double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    double q = g * i - e * k, s = e * j - f * i;

    double inv_denom = 1.0 / (a * m + b * q + c * s);

    double e1 = d * m - b * n - c * p;
    double beta = e1 * inv_denom;

    if (beta < 0.0)
    {
        return false;
    }

    double r = e * l - h * i;
    double e2 = a * n + d * q + c * r;
    double gamma = e2 * inv_denom;

    if (gamma < 0.0)
    {
        return false;
    }

    if (beta + gamma > 1.0)
    {
        return false;
    }

    double e3 = a * p - b * r + d * s;
    double t = e3 * inv_denom;

    if (t < std::numeric_limits<double>::epsilon())
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
    const double delta = std::numeric_limits<double>::epsilon();
    Interval ix = Interval(std::min(std::min(v0.x, v1.x), v2.x) - delta, std::max(std::max(v0.x, v1.x), v2.x) + delta);
    Interval iy = Interval(std::min(std::min(v0.y, v1.y), v2.y) - delta, std::max(std::max(v0.y, v1.y), v2.y) + delta);
    Interval iz = Interval(std::min(std::min(v0.z, v1.z), v2.z) - delta, std::max(std::max(v0.z, v1.z), v2.z) + delta);

    return AABB(ix, iy, iz);
}

std::vector<std::shared_ptr<Triangle>> raytracer::tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps, std::shared_ptr<Material> mat)
{
    std::vector<std::shared_ptr<Triangle>> objects;
    // define the top triangles which all touch the north pole
    int k = 1;

    for (int j = 0; j <= horizontal_steps - 1; j++)
    {
        // define vertices

        Point3 v0(0, 1, 0); // top (north pole)

        Point3 v1(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // bottom left
                  cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

        Point3 v2(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // bottom  right
                  cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

        Triangle triangle_ptr(v0, v1, v2, mat);
        objects.push_back(std::make_shared<Triangle>(triangle_ptr));
    }

    // define the bottom triangles which all touch the south pole

    k = vertical_steps - 1;

    for (int j = 0; j <= horizontal_steps - 1; j++)
    {
        // define vertices

        Point3 v0(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left
                  cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

        Point3 v1(0, -1, 0); // bottom (south pole)

        Point3 v2(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // top right
                  cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

        Triangle triangle_ptr(v0, v1, v2, mat);
        objects.push_back(std::make_shared<Triangle>(triangle_ptr));
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

            Triangle triangle_ptr1(v0, v1, v2, mat);
            objects.push_back(std::make_shared<Triangle>(triangle_ptr1));

            // define the second triangle

            // vertices

            v0 = Point3(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // top right, use k, j + 1
                        cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

            v1 = Point3(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left,     use k, j
                        cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

            v2 = Point3(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), // bottom  right, use k + 1, j + 1
                        cos(pi * (k + 1) / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps));

            Triangle triangle_ptr2(v0, v1, v2, mat);
            objects.push_back(std::make_shared<Triangle>(triangle_ptr2));
        }
    }
    return objects;
}
