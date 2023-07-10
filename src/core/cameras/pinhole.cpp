#include "pinhole.h"

using namespace raytracer;

Pinhole::Pinhole(Point3 eye_p, Point3 lookat):
    Camera(eye_p, lookat),
    d(1),
    zoom(1)
{}

void Pinhole::set_view_distance(const double dist){
    d = dist;
}

float Pinhole::get_distance_from_vp() const{
    return d;
}

void Pinhole::set_zoom(const double zoom){
    this->zoom = zoom;
}

float Pinhole::get_zoom() const{
    return zoom;
}

raytracer::Ray Pinhole::get_ray(const Point2& pixel_point) const {
    Vector3 dir = Normalize(pixel_point.x*u + pixel_point.y*v - d*w);
    return raytracer::Ray(eye, dir);
}