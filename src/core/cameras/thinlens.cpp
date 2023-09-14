#include "thinlens.h"
#include "pinhole.h"
#include "pure_random.h"
using namespace raytracer;

ThinLens::ThinLens(Point3 eye_p, Point3 lookat) : raytracer::Pinhole(eye_p, lookat),
                                                  sampler_ptr(new raytracer::PureRandom(4))
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

Vector3 ThinLens::ray_direction(const Point2 &pixel_point, const Point2 &lens_point) const
{
    Point2 p;
    p.x = pixel_point.x * f / d;
    p.y = pixel_point.y * f / d;
    Vector3 dir = (p.x - lens_point.x) * u + (p.y - lens_point.y) * v - f * w;
    dir = Normalize(dir);

    return dir;
}

Ray ThinLens::get_ray(const Point2& pixel_point) const {
    Point2 dp = this->sampler_ptr->sample_unit_disk();
    Point2 lp = dp * this->lens_radius;
    Vector3 dir = ray_direction(pixel_point, lp);
    return Ray(eye, dir);
}