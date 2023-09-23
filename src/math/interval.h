#pragma once
#include <cmath>
namespace raytracer
{
    class Interval
    {
    public:
        Interval() {}

        Interval(const Interval &a, const Interval &b);

        Interval(double min, double max);
        double size() const;

        Interval expand(double delta) const;

        double min = 0, max = 0;
    };
}