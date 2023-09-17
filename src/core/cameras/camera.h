#pragma once
#include "maths.h"
#include "ray.h"
namespace raytracer
{
    class Camera
    {
    protected:
        raytracer::Point3 eye;
        raytracer::Point3 look_at;
        raytracer::Vector3 up;
        raytracer::Vector3 u, v, w;
        double exposure_time;

    public:
        Camera(raytracer::Point3 eye_p = raytracer::Point3(0, 0, 1), raytracer::Point3 lookat = raytracer::Point3(0, 0, -1));
        virtual ~Camera();

        void compute_uvw();

        void set_eye(const raytracer::Point3 eye_p);
        void set_eye(const double x, const double y, const double z);
        raytracer::Point3 get_eye() const;

        void set_lookat(const raytracer::Point3 lookat);
        void set_lookat(const double x, const double y, const double z);
        raytracer::Point3 get_lookat() const;

        void set_up(const raytracer::Vector3 up_d);
        raytracer::Vector3 get_up() const;

        void set_exposure_time(const double exposure_t);
        double get_exposure_time() const;

        virtual Ray get_ray(const Point2 &pixel_point) const = 0;

        virtual double get_pixel_size() const = 0;
    };
}