#pragma once
#include <limits>
#include <random>
#include <vector>
#include <cmath>

#include <Eigen/Dense>

#include "color.h"

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;
const float inv_pi = 0.31830988618;

inline float deg_to_rad(float degrees)
{
    return degrees * pi / 180.0;
}

inline float rad_to_deg(float rad)
{
    return rad * 180.0 / pi;
}

inline float clip(float x, float min, float max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

inline int clip(int x, int min, int max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

inline float random_float()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    return distribution(gen);
}

inline int random_int()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> distribution(0, std::numeric_limits<int>::max());
    return distribution(gen);
}

inline float random_float(float min, float max)
{
    return min + (max - min) * random_float();
}

inline int random_int(int min, int max)
{
    return min + (random_int() % (max - min + 1));
}

inline bool NearZero(Eigen::Vector3f v)
{
    // Return true if the vector is close to zero in all dimensions.
    auto eps = 1e-8;
    return (fabs(v.x()) < eps) && (fabs(v.y()) < eps) && (fabs(v.z()) < eps);
}

inline Eigen::Vector3f Clamp(Eigen::Vector3f v, float minVal, float maxVal)
{
    v.x() = (float)std::min(std::max(v.x(), minVal), maxVal);
    v.y() = (float)std::min(std::max(v.y(), minVal), maxVal);
    v.z() = (float)std::min(std::max(v.z(), minVal), maxVal);
    return v;
}

inline Eigen::Vector3f Reflect(const Eigen::Vector3f &v, const Eigen::Vector3f &n)
{
    return v - 2 * v.dot(n) * n;
}

inline Eigen::Vector3f Refract(const Eigen::Vector3f &uv, const Eigen::Vector3f &n, float etai_over_etat)
{
    double cos_theta = std::min((-uv).dot(n), 1.0f);
    Eigen::Vector3f r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Eigen::Vector3f r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squaredNorm())) * n;
    return r_out_perp + r_out_parallel;
}

inline float power_heuristic(int nf, float fPdf, int ng, float gPdf) {
    float f = nf * fPdf, g = ng * gPdf;
    return (f * f) / (f * f + g * g);
}