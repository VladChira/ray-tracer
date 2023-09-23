#include "point_light.h"
#include "interval.h"
using namespace raytracer;

PointLight::PointLight(double ls, Color3 c, Vector3 loc)
{
    this->ls = ls;
    this->color = c;
    this->location = loc;
}

Vector3 PointLight::get_direction(const Ray &r_in, const HitInfo &rec) const
{
    return Normalize(location - rec.p);
}

Color3 PointLight::L(const Ray &r_in, const HitInfo &rec) const
{
    return ls * color;
}

double PointLight::G(const Ray &r_in, const HitInfo &rec) const
{
    return 1.0;
}

double PointLight::pdf(const Ray &r_in, const HitInfo &rec) const
{
    return 1.0;
}

bool PointLight::in_shadow(const Ray &r, const HitInfo &rec) const
{
    double d = (location - r.origin).Length();
    HitInfo rec2(rec.world);
    for (int i = 0 ; i < rec.world.objects.size(); i++)
    {
        bool hit = rec.world.hit_objects(r, Interval(0.0001, infinity), rec2);
        if (hit && rec2.t < d) return true;
    }
    return false;
}