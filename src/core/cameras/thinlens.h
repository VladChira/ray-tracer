#pragma once
#include "maths.h"
#include "sampler.h"
#include "pinhole.h"

namespace raytracer
{
    class ThinLens : public raytracer::Pinhole
    {
    public:
        ThinLens(raytracer::Point3 eye_p = raytracer::Point3(0, 0, 1), raytracer::Point3 lookat = raytracer::Point3(0, 0, -1));
        ~ThinLens();

        void set_sampler(raytracer::Sampler *sp);
        raytracer::Sampler *get_sampler();

        void set_lens_radius(const float l_r);
        float get_lens_radius();

        void set_focal_dist(const float focal_d);
        float get_focal_dist();

        raytracer::Vector3 ray_direction(const raytracer::Point2 &pixel_point, const raytracer::Point2 &lens_point = raytracer::Point2(0, 0)) const;

    private:
        float lens_radius;
        float f; // Focal plane distance
        raytracer::Sampler *sampler_ptr;
    };
}