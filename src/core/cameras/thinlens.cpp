#include "thinlens.h"
#include "pinhole.h"
#include "multijittered.h"
using namespace raytracer;

ThinLens::ThinLens(Eigen::Vector3f eye_p, Eigen::Vector3f lookat) : raytracer::Pinhole(eye_p, lookat),
                                                  sampler_ptr(new raytracer::MultiJittered(4))
{
    sampler_ptr->map_samples_to_unit_disk();
}

ThinLens::~ThinLens()
{
    if (sampler_ptr)
    {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
}

void ThinLens::set_sampler(Sampler *sp)
{
    if (sampler_ptr)
    {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    sampler_ptr = sp;
    sampler_ptr->map_samples_to_unit_disk();
}

Sampler *ThinLens::get_sampler()
{
    return sampler_ptr;
}

void ThinLens::set_lens_radius(const float l_r)
{
    lens_radius = l_r;
}

float ThinLens::get_lens_radius()
{
    return lens_radius;
}

void ThinLens::set_focal_dist(const float focal_d)
{
    f = focal_d;
}

float ThinLens::get_focal_dist()
{
    return f;
}

Eigen::Vector3f ThinLens::ray_direction(const Eigen::Vector2f &pixel_point, const Eigen::Vector2f &lens_point) const
{
    Eigen::Vector2f p;
    p.x() = pixel_point.x() * f / d;
    p.y() = pixel_point.y() * f / d;
    Eigen::Vector3f dir = (p.x() - lens_point.x()) * u + (p.y() - lens_point.y()) * v - f * w;
    dir.normalize();
    return dir;
}

Ray ThinLens::get_ray(const Eigen::Vector2f &pixel_point) const
{
    Eigen::Vector2f dp = this->sampler_ptr->sample_unit_disk();
    Eigen::Vector2f lp = dp * this->lens_radius;
    Eigen::Vector3f dir = ray_direction(pixel_point, lp);
    return Ray(eye, dir);
}