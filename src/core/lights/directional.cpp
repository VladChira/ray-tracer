#include "directional.h"

using namespace raytracer;

Directional::Directional(double ls, Color3 c, Vector3 dir)
{
    this->ls = ls;
    this->color = c;
    this->direction = dir;
}

Vector3 Directional::get_direction(const Ray &r_in, const HitInfo &rec) const
{
    return direction;
}

Color3 Directional::L(const Ray &r_in, const HitInfo &rec) const
{
    return ls * color;
}

double Directional::G(const Ray &r_in, const HitInfo &rec) const
{
    return 1.0;
}

double Directional::pdf(const Ray &r_in, const HitInfo &rec) const
{
    return 1.0;
}