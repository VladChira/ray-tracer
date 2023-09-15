#pragma once
#include "camera.h"
#include "maths.h"
#include "ray.h"

namespace raytracer
{
    class Pinhole : public Camera
    {
    public:
        Pinhole(raytracer::Point3 eye_p = raytracer::Point3(0, 0, 1), raytracer::Point3 lookat = raytracer::Point3(0, 0, -1));

        void set_view_distance(const double dist);
        double get_view_distance() const;

        void set_fov(const double fov_deg);
        double get_fov() const;

        double get_pixel_size() const;

        raytracer::Ray get_ray(const raytracer::Point2 &pixel_point) const;

        void compute_pixel_size(double image_width, double image_height);

    protected:
        double d;          // Distance from view-plane
        double fov;        // The field of view of the camera
        double pixel_size; // The size of a single pixel
    };
}