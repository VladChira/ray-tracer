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

        bool scatter(const Ray &r_in, const HitInfo &rec, Color3 &attenuation, Ray &scattered) const override
        {
            attenuation = Color3(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

            Vector3 unit_direction = Normalize(r_in.direction);
            double cos_theta = fmin(Dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Vector3 direction;
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = Reflect(unit_direction, rec.normal);
            else
                direction = Refract(unit_direction, rec.normal, refraction_ratio);

            scattered = Ray(rec.p, direction);
            return true;
        }

    private:
        double ir; // index of refraction

        static double reflectance(double cosine, double ref_idx)
        {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
    };
}