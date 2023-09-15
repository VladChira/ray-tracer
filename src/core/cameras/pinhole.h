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
        float get_distance_from_vp() const;

        void set_zoom(const double zoom);
        float get_zoom() const;

        raytracer::Ray get_ray(const raytracer::Point2 &pixel_point) const;

        void initialize(double aspect_ratio, double image_width);

    protected:
        double d;    // Distance from view-plane
        double zoom; // Zoom factor

    public:
        double image_width, image_height;
        Point3 center, pixel_delta_u, pixel_delta_v, pixel00_loc;
    };
}