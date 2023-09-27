#pragma once
#include "light.h"
#include "hit_info.h"
namespace raytracer
{
    class PointLight : public Light
    {
    private:
        double ls;
        Color3 color;
        Vector3 location;

    public:
        PointLight(double ls, Color3 c, Vector3 dir);

        Vector3 get_direction(const Ray &r_in, const HitInfo &rec) override;
        Color3 L(const Ray &r_in, const HitInfo &rec) const override;
        double G(const Ray &r_in, const HitInfo &rec) const override;
        double pdf(const Ray &r_in, const HitInfo &rec) const override;
        bool in_shadow(const Ray &r, const HitInfo &rec) const override;
    };
}
