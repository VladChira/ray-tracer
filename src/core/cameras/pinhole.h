#pragma once
#include "camera.h"
#include "maths.h"
#include "ray.h"

namespace raytracer
{
    class Pinhole : public Camera
    {
    public:
        Pinhole(raytracer::Point3 eye_p, raytracer::Point3 lookat);
        
        void compute_pixel_size(double image_width, double image_height);
        void set_fov(const double fov_deg);
        double get_fov() const;

        double get_pixel_size() const override;

        raytracer::Ray get_ray(const raytracer::Point2 &pixel_point) const override;



    protected:
        double d;          // Distance from view-plane
        double fov;        // The field of view of the camera
        double pixel_size; // The size of a single pixel
    };
}