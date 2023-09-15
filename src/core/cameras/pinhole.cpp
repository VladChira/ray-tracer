#include "pinhole.h"

using namespace raytracer;

Pinhole::Pinhole(Point3 eye_p, Point3 lookat) : Camera(eye_p, lookat),
                                                d(1),
                                                zoom(1)
{
}

void Pinhole::set_view_distance(const double dist)
{
    d = dist;
}

float Pinhole::get_distance_from_vp() const
{
    return d;
}

void Pinhole::set_zoom(const double zoom)
{
    this->zoom = zoom;
}

float Pinhole::get_zoom() const
{
    return zoom;
}

raytracer::Ray Pinhole::get_ray(const Point2 &pixel_point) const
{
    Vector3 dir = Normalize(pixel_point.x * u + pixel_point.y * v - d * w);
    return raytracer::Ray(eye, dir);
}

void Pinhole::initialize(double aspect_ratio, double image_width)
{
    this->image_width = image_width;
    this->image_height = static_cast<int>(image_width / aspect_ratio);
    this->image_height = (image_height < 1) ? 1 : image_height;

    this->center = Point3(0, 0, 0);

    // Determine viewport dimensions.
    auto focal_length = 3;
    auto viewport_height = 1.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = Vector3(viewport_width, 0, 0);
    auto viewport_v = Vector3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    this->pixel_delta_u = viewport_u / image_width;
    this->pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left =
        center - Vector3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    this->pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}