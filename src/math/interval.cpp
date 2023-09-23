#include "interval.h"
using namespace raytracer;

Interval::Interval(const Interval &a, const Interval &b)
{
    this->min = fmin(a.min, b.min);
    this->max = fmax(a.max, b.max);
}

Interval::Interval(double min, double max)
{
    this->min = min;
    this->max = max;
}

double Interval::size() const
{
    return max - min;
}

Interval Interval::expand(double delta) const
{
    auto padding = delta / 2;
    return Interval(min - padding, max + padding);
}