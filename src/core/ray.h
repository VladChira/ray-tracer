#pragma once
#include "maths.h"

namespace raytracer
{
    class Ray
    {
    public:
        Vector3 origin;
        Vector3 direction;
        double tMax;

        Ray()
        {
            this->tMax = infinity;
            this->origin = Vector3(0.0, 0.0, 0.0);
            this->direction = Vector3(0.0, 0.0, 1.0);
        }

        Ray(const Vector3 &o, const Vector3 &d)
        {
            this->tMax = INFINITY;
            this->origin = o;
            this->direction = d;
        }

        Vector3 at(const double t) const
        {
            assert(t <= tMax);
            return origin + t * direction;
        }
    };

    inline std::ostream &operator<<(std::ostream &os, const Ray &r)
    {
        os << r.origin << "    " << r.direction;
        return os;
    }
}