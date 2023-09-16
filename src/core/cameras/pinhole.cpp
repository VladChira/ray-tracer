#include "pinhole.h"
#include "vector3.h"
using namespace raytracer;

Pinhole::Pinhole(raytracer::Point3 eye_p, raytracer::Point3 lookat)
{
    this->eye = eye_p;
    this->look_at = lookat;
    this->d = (eye_p - lookat).Length();
}

double Pinhole::get_fov() const
{
    return this->fov;
}

void Pinhole::set_fov(double fov_deg)
{
    this->fov = fov_deg;
}

double Pinhole::get_pixel_size() const
{
    return this->pixel_size;
}

raytracer::Ray Pinhole::get_ray(const Point2 &pixel_point) const
{
    Vector3 dir = Normalize(pixel_point.x * u + pixel_point.y * v - d*w);
    return raytracer::Ray(eye, dir);
}

void Pinhole::compute_pixel_size(double image_width, double image_height)
{
    const double aspect_ratio = image_width / image_height;
    const double h = 2 * this->d * tan(deg_to_rad(this->fov) / 2);
    const double w = aspect_ratio * h;
    this->pixel_size = w / image_width;
}