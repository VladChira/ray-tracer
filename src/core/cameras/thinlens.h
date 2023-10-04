#pragma once
#include "utilities.h"
#include "sampler.h"
#include "pinhole.h"

namespace raytracer
{
    class ThinLens : public raytracer::Pinhole
    {
    public:
        ThinLens(Eigen::Vector3f eye_p = Eigen::Vector3f(0, 0, 1), Eigen::Vector3f lookat = Eigen::Vector3f(0, 0, -1));
        ~ThinLens();

        void set_sampler(raytracer::Sampler *sp);
        raytracer::Sampler *get_sampler();

        void set_lens_radius(const float l_r);
        float get_lens_radius();

        void set_focal_dist(const float focal_d);
        float get_focal_dist();

        Eigen::Vector3f ray_direction(const Eigen::Vector2f &pixel_point, const Eigen::Vector2f &lens_point = Eigen::Vector2f(0, 0)) const;

        raytracer::Ray get_ray(const Eigen::Vector2f &pixel_point) const;

    private:
        float lens_radius;
        float f; // Focal plane distance
        raytracer::Sampler *sampler_ptr;
    };
}