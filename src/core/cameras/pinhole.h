#pragma once
#include "camera.h"
#include "utilities.h"
#include "ray.h"

namespace raytracer
{
    class Pinhole : public Camera
    {
    public:
        Pinhole(Eigen::Vector3f eye_p, Eigen::Vector3f lookat);
        
        void compute_pixel_size(float image_width, float image_height);
        void set_fov(const float fov_deg);
        float get_fov() const;

        float get_pixel_size() const override;

        raytracer::Ray get_ray(const Eigen::Vector2f &pixel_point) const override;



    protected:
        float d;          // Distance from view-plane
        float fov;        // The field of view of the camera
        float pixel_size; // The size of a single pixel
    };
}