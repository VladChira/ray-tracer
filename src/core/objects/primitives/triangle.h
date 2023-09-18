#include "geometric_object.h"

namespace raytracer
{
    class Triangle : public GeometricObject
    {
    public:
        Point3 v0, v1, v2;
        Normal3 normal;

        Triangle();

        Triangle(const Point3 &a, const Point3 &b, const Point3 &c, std::shared_ptr<Material> mat);

        bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const override;

        AABB bounding_box() const override;
    };
}