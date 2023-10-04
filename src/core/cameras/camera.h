#pragma once
#include "utilities.h"
#include "ray.h"
namespace raytracer
{
    class Camera
    {
    protected:
        Eigen::Vector3f eye;
        Eigen::Vector3f look_at;
        Eigen::Vector3f up;
        Eigen::Vector3f u, v, w;
        float exposure_time;

    public:
        Camera(Eigen::Vector3f eye_p = Eigen::Vector3f(0, 0, 1), Eigen::Vector3f lookat = Eigen::Vector3f(0, 0, -1));
        virtual ~Camera();

        void compute_uvw();

        void set_eye(const Eigen::Vector3f eye_p);
        void set_eye(const float x, const float y, const float z);
        Eigen::Vector3f get_eye() const;

        void set_lookat(const Eigen::Vector3f lookat);
        void set_lookat(const float x, const float y, const float z);
        Eigen::Vector3f get_lookat() const;

        void set_up(const Eigen::Vector3f up_d);
        Eigen::Vector3f get_up() const;

        void set_exposure_time(const float exposure_t);
        float get_exposure_time() const;

        virtual Ray get_ray(const Eigen::Vector2f &pixel_point) const = 0;

        virtual float get_pixel_size() const = 0;
    };
}