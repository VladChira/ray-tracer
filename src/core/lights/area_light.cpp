#include "area_light.h"
using namespace raytracer;

AreaLight::AreaLight() {}

Color3 AreaLight::L(const Ray &r_in, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi) const
{
    double ndotd = Dot(-light_normal, wi);

    if (ndotd > 0.0f) {
        Color3 c = emissive_mat->get_Le(r_in, rec);
        return c;
    } else {
        return Color3::black;
    }
}

bool AreaLight::in_shadow(const Ray &ray, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi) const {
    double t;
    int num_objects = rec.world.objects.size();
    double ts = Dot((sample_point - ray.origin), ray.direction);
    HitInfo rec2(rec.world);
    for (int i = 0 ; i < rec.world.objects.size(); i++)
    {
        bool hit = rec.world.hit_objects(ray, Interval(0.0001, infinity), rec2);
        if (hit && rec2.t < ts) return true;
    }
    return false;
}

double AreaLight::G(const Ray &ray, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi) const {
    double ndotd = Dot(-light_normal, wi);
    double d2 = (sample_point - rec.p).LengthSquared();

    return ndotd / d2;
}

double AreaLight::pdf(const Ray &ray, const HitInfo &rec) const 
{
    return object->pdf(ray, rec);
}


Vector3 AreaLight::get_direction(const Ray &r_in, const HitInfo &rec, Point3 &sample_point, Normal3 &light_normal, Vector3 &wi)
{
    sample_point = object->sample();  // used in the G function
    light_normal = object->get_normal(sample_point);
    wi = Normalize(sample_point - rec.p);  // used in the G function
    return wi;
}