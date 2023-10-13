#pragma once
#include "material.h"
#include "geometric_object.h"

namespace raytracer
{
    class Transparent : public Material
    {
    public:
        Transparent(double ir)
        {
            this->ir = ir;
        }

        bool scatter(const Ray &r_in, const HitInfo &rec, Color &attenuation, Ray &scattered) const override
        {
            attenuation = Color(1.0, 1.0, 1.0);
            float refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

            Eigen::Vector3f unit_direction = (r_in.direction).normalized();
            float cos_theta = fmin((-unit_direction).dot(rec.normal), 1.0);
            float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Eigen::Vector3f direction;
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float())
                direction = Reflect(unit_direction, rec.normal);
            else
                direction = Refract(unit_direction, rec.normal, refraction_ratio);

            scattered = Ray(rec.p + 0.00001 * direction, direction);
            return true;
        }

        Color shade(const raytracer::Ray &r_in, HitInfo &rec) override
        {
            return Color(0, 0, 0);
        }

    private:
        float ir; // index of refraction

        static float reflectance(float cosine, float ref_idx)
        {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
    };
}