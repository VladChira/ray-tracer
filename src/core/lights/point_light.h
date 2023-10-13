// #pragma once
// #include "light.h"
// #include "hit_info.h"
// namespace raytracer
// {
//     class PointLight : public Light
//     {
//     private:
//         float ls;
//         Color color;
//         Eigen::Vector3f location;

//     public:
//         PointLight(float ls, Color c, Eigen::Vector3f dir);

//         Eigen::Vector3f get_direction(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) override;
//         Color L(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const override;
//         float G(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const override;
//         float pdf(const Ray &r_in, const HitInfo &rec) const override;
//         bool in_shadow(const Ray &r, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const override;
//     };
// }
