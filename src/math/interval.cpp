#include "interval.h"
using namespace raytracer;

Interval::Interval(const Interval &a, const Interval &b)
{
    this->min = fmin(a.min, b.min);
    this->max = fmax(a.max, b.max);
}

Interval::Interval(float min, float max)
{
    this->min = min;
    this->max = max;
}

float Interval::size() const
{
    return max - min;
}

Interval Interval::expand(float delta) const
{
    auto padding = delta / 2;
    return Interval(min - padding, max + padding);
}