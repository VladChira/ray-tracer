#include "camera.h"
using namespace raytracer;

Camera::Camera(Eigen::Vector3f eye_p, Eigen::Vector3f lookat) : eye(eye_p),
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
    w = (eye - look_at).normalized();
    u = (up.cross(w)).normalized();
    v = w.cross(u);
}

void Camera::set_eye(const Eigen::Vector3f eye_p)
{
    eye = eye_p;
}

void Camera::set_eye(const float x, const float y, const float z)
{
    eye = Eigen::Vector3f(x, y, z);
}

Eigen::Vector3f Camera::get_eye() const
{
    return eye;
}

void Camera::set_lookat(const Eigen::Vector3f lookat)
{
    look_at = lookat;
}

void Camera::set_lookat(const float x, const float y, const float z)
{
    look_at = Eigen::Vector3f(x, y, z);
}

Eigen::Vector3f Camera::get_lookat() const
{
    return look_at;
}

void Camera::set_up(const Eigen::Vector3f up_d)
{
    up = up_d;
}

Eigen::Vector3f Camera::get_up() const
{
    return up;
}

void Camera::set_exposure_time(const float exposure_t)
{
    exposure_time = exposure_t;
}

float Camera::get_exposure_time() const
{
    return exposure_time;
}