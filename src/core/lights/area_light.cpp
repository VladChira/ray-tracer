#include "area_light.h"
using namespace raytracer;

AreaLight::AreaLight() {}

Color AreaLight::L(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const
{
    float ndotd = (-light_normal).dot(wi);

    if (ndotd > 0.0f) {
        Color c = emissive_mat->get_Le(r_in, rec);
        return c;
    } else {
        return Color::black;
    }
}

bool AreaLight::occluded(const Ray &ray, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const {
    float t;
    int num_objects = rec.world.objects.size();
    float ts = (sample_point - ray.origin).dot(ray.direction);
    HitInfo rec2(rec.world);
    for (int i = 0 ; i < rec.world.objects.size(); i++)
    {
        bool hit = rec.world.hit_objects(ray, Interval(0.0001, infinity), rec2);
        if (hit && rec2.t < ts) return true;
    }
    return false;
}

// float AreaLight::G(const Ray &ray, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const {
//     float ndotd = (-light_normal).dot(wi);
//     float d2 = (sample_point - rec.p).squaredNorm();

//     return ndotd / d2;
// }

float AreaLight::pdf(const Ray &ray, const HitInfo &rec) const 
{
    return object->pdf(ray, rec);
}

Color AreaLight::Sample_Li(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi, float &pdf, bool &in_shadow) const
{
    sample_point = object->sample();
    light_normal = object->get_normal(sample_point);
    wi = (sample_point - rec.p).normalized();
    pdf = this->pdf(r_in, rec);
    if (wi.dot(rec.normal) < 0.0) return Color::black;
    // Create a shadow ray and check occlusion
    Ray shadow_ray = Ray(rec.p, wi);
    in_shadow = occluded(shadow_ray, rec, sample_point, light_normal, wi);
    return L(r_in, rec, sample_point, light_normal, wi);
}


// Eigen::Vector3f AreaLight::get_direction(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi)
// {
//     sample_point = object->sample();  // used in the G function
//     light_normal = object->get_normal(sample_point);
//     wi = (sample_point - rec.p).normalized();  // used in the G function
//     return wi;
// }