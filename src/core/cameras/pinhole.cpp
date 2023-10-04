#include "pinhole.h"
using namespace raytracer;

Pinhole::Pinhole(Eigen::Vector3f eye_p, Eigen::Vector3f lookat)
{
    this->eye = eye_p;
    this->look_at = lookat;
    this->d = (eye_p - lookat).norm();
}

float Pinhole::get_fov() const
{
    return this->fov;
}

void Pinhole::set_fov(float fov_deg)
{
    this->fov = fov_deg;
}

float Pinhole::get_pixel_size() const
{
    return this->pixel_size;
}

raytracer::Ray Pinhole::get_ray(const Eigen::Vector2f &pixel_point) const
{
    Eigen::Vector3f dir = (pixel_point.x() * u + pixel_point.y() * v - d*w).normalized();
    return raytracer::Ray(eye, dir);
}

void Pinhole::compute_pixel_size(float image_width, float image_height)
{
    const float aspect_ratio = image_width / image_height;
    const float h = 2 * this->d * tan(deg_to_rad(this->fov) / 2);
    const float w = aspect_ratio * h;
    this->pixel_size = w / image_width;
}