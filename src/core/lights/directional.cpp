#include "directional.h"

using namespace raytracer;

Directional::Directional(double ls, Color3 c, Vector3 dir)
{
    this->ls = ls;
    this->color = c;
    this->direction = dir;
}

Vector3 Directional::get_direction(const Ray &r_in, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi)
{
    return direction;
}

Color3 Directional::L(const Ray &r_in, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi) const
{
    return ls * color;
}

double Directional::G(const Ray &r_in, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi) const
{
    return 1.0;
}

double Directional::pdf(const Ray &r_in, const HitInfo &rec) const
{
    return 1.0;
}

bool Directional::in_shadow(const Ray &r, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi) const
{
    return false;
}
