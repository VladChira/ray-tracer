// #include "point_light.h"
// #include "interval.h"
// using namespace raytracer;

// PointLight::PointLight(float ls, Color c, Eigen::Vector3f loc)
// {
//     this->ls = ls;
//     this->color = c;
//     this->location = loc;
// }

// Eigen::Vector3f PointLight::get_direction(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi)
// {
//     return (location - rec.p).normalized();
// }

// Color PointLight::L(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const
// {
//     return color * ls;
// }

// float PointLight::G(const Ray &r_in, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const
// {
//     return 1.0;
// }

// float PointLight::pdf(const Ray &r_in, const HitInfo &rec) const
// {
//     return 1.0;
// }

// bool PointLight::in_shadow(const Ray &r, const HitInfo &rec, Eigen::Vector3f &sample_point, Eigen::Vector3f &light_normal, Eigen::Vector3f &wi) const
// {
//     float d = (location - r.origin).norm();
//     HitInfo rec2(rec.world);
//     for (int i = 0 ; i < rec.world.objects.size(); i++)
//     {
//         bool hit = rec.world.hit_objects(r, Interval(0.0001, infinity), rec2);
//         if (hit && rec2.t < d) return true;
//     }
//     return false;
// }