#pragma once
#include "utilities.h"

namespace raytracer
{
    class Ray
    {
    public:
        Eigen::Vector3f origin;
        Eigen::Vector3f direction;
        float tMax;

        Ray()
        {
            this->tMax = infinity;
            this->origin = Eigen::Vector3f(0.0, 0.0, 0.0);
            this->direction = Eigen::Vector3f(0.0, 0.0, 1.0);
        }

        Ray(const Eigen::Vector3f &o, const Eigen::Vector3f &d)
        {
            this->tMax = infinity;
            this->origin = o;
            this->direction = d;
        }

        Eigen::Vector3f at(const float t) const
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