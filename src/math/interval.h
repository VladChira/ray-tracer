#pragma once
#include <cmath>
namespace raytracer
{
    class Interval
    {
    public:
        Interval() {}

        Interval(const Interval &a, const Interval &b);

        Interval(float min, float max);
        float size() const;

        Interval expand(float delta) const;

        float min = 0, max = 0;
    };
}