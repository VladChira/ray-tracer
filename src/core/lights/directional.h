#pragma once
#include "light.h"

namespace raytracer
{
    class Directional : public Light
    {
    private:
        double ls;
        Color3 color;
        Vector3 direction;

    public:
        Directional(double ls, Color3 c, Vector3 dir);

        Vector3 get_direction(const Ray &r_in, const HitInfo &rec) const override;
        Color3 L(const Ray &r_in, const HitInfo &rec) const override;
        double G(const Ray &r_in, const HitInfo &rec) const override;
        double pdf(const Ray &r_in, const HitInfo &rec) const override;
    };
}