#include "camera.h"
using namespace raytracer;

Camera::Camera(Point3 eye_p, Point3 lookat) : eye(eye_p),
                                              look_at(lookat),
                                              up(0, 1, 0),
                                              exposure_time(1)
{
    compute_uvw();
}

Camera::~Camera()
{
}

void Camera::compute_uvw()
{
    w = Normalize(eye - look_at);
    u = Normalize(Cross(up, w));
    v = Cross(w, u);
}

void Camera::set_eye(const Point3 eye_p)
{
    eye = eye_p;
}

void Camera::set_eye(const double x, const double y, const double z)
{
    eye = Point3(x, y, z);
}

Point3 Camera::get_eye() const
{
    return eye;
}

void Camera::set_lookat(const Point3 lookat)
{
    look_at = lookat;
}

void Camera::set_lookat(const double x, const double y, const double z)
{
    look_at = Point3(x, y, z);
}

Point3 Camera::get_lookat() const
{
    return look_at;
}

void Camera::set_up(const Vector3 up_d)
{
    up = up_d;
}

Vector3 Camera::get_up() const
{
    return up;
}

void Camera::set_exposure_time(const double exposure_t)
{
    exposure_time = exposure_t;
}

double Camera::get_exposure_time() const
{
    return exposure_time;
}