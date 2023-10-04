#include "directional.h"

using namespace raytracer;

Directional::Directional(float ls, Color c, Eigen::Vector3f dir)
{
    this->ls = ls;
    this->color = c;
    this->direction = dir;
}

Eigen::Vector3f Directional::get_direction(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi)
{
    return direction;
}

Color Directional::L(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const
{
    return color * ls;
}

float Directional::G(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const
{
    return 1.0;
}

float Directional::pdf(const Ray &r_in, const HitInfo &rec) const
{
    return 1.0;
}

bool Directional::in_shadow(const Ray &r, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const
{
    return false;
}
