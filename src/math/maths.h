#pragma once
#include "vector3.h"
#include "vector2.h"
#include <limits>
#include <random>
#include <vector>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double inv_pi = 0.31830988618;

inline double deg_to_rad(double degrees)
{
    return degrees * pi / 180.0;
}

inline double rad_to_deg(double rad)
{
    return rad * 180.0 / pi;
}

inline double clip(double x, double min, double max)
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

inline double random_double()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(gen);
}

inline unsigned int random_uint()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<unsigned int> distribution(0, std::numeric_limits<unsigned int>::max());
    return distribution(gen);
}

inline int random_int()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> distribution(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    return distribution(gen);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

inline unsigned int random_uint(unsigned int min, unsigned int max) {
    return min + (random_uint() % (max - min + 1));
}