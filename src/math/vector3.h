#pragma once
#include <assert.h>
#include <cmath>
#include <iostream>
namespace raytracer
{
    class Vector3
    {
    public:
        double x, y, z;

        double operator[](int i) const
        {
            assert(i >= 0 && i <= 2);
            if (i == 0)
                return x;
            if (i == 1)
                return y;
            return z;
        }

        double &operator[](int i)
        {
            assert(i >= 0 && i <= 2);
            if (i == 0)
                return x;
            if (i == 1)
                return y;
            return z;
        }

        Vector3()
        {
            x = y = z = 0;
        }

        Vector3(double x, double y, double z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Vector3(const Vector3 &v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }

        Vector3 &operator=(const Vector3 &v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }

        Vector3 operator+(const Vector3 &v) const
        {
            return Vector3(x + v.x, y + v.y, z + v.z);
        }
        Vector3 &operator+=(const Vector3 &v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vector3 operator-(const Vector3 &v) const
        {
            return Vector3(x - v.x, y - v.y, z - v.z);
        }

        Vector3 &operator-=(const Vector3 &v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        bool operator==(const Vector3 &v) const
        {
            return x == v.x && y == v.y && z == v.z;
        }

        bool operator!=(const Vector3 &v) const
        {
            return x != v.x || y != v.y || z != v.z;
        }

        Vector3 operator*(double s) const
        {
            return Vector3(s * x, s * y, s * z);
        }

        Vector3 &operator*=(double s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        Vector3 operator/(double f) const
        {
            assert(f != 0);
            double inv = 1 / f;
            return Vector3(x * inv, y * inv, z * inv);
        }

        Vector3 &operator/=(double f)
        {
            assert(f != 0);
            double inv = 1 / f;
            x *= inv;
            y *= inv;
            z *= inv;
            return *this;
        }

        Vector3 operator-() const { return Vector3(-x, -y, -z); }
        double LengthSquared() const { return x * x + y * y + z * z; }
        double Length() const { return std::sqrt(LengthSquared()); }
    };

    using Point3 = Vector3;
    using Color3 = Vector3;
    using Normal3 = Vector3;

    inline std::ostream &operator<<(std::ostream &os, const Vector3 &v)
    {
        os << v.x << " " << v.y << " " << v.z;
        return os;
    }

    inline Vector3 operator*(double s, const Vector3 &v)
    {
        return v * s;
    }

    inline Vector3 Abs(const Vector3 &v)
    {
        return Vector3(std::abs(v.x), std::abs(v.y), std::abs(v.z));
    }

    inline double Dot(const Vector3 &v1, const Vector3 &v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    inline double AbsDot(const Vector3 &v1, const Vector3 &v2)
    {
        return std::abs(Dot(v1, v2));
    }

    inline Vector3 Cross(const Vector3 &v1, const Vector3 &v2)
    {
        double v1x = v1.x, v1y = v1.y, v1z = v1.z;
        double v2x = v2.x, v2y = v2.y, v2z = v2.z;
        return Vector3((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
                       (v1x * v2y) - (v1y * v2x));
    }

    inline Vector3 Normalize(const Vector3 &v)
    {
        return v / v.Length();
    }

    inline double MinComponent(const Vector3 &v)
    {
        return std::min(v.x, std::min(v.y, v.z));
    }

    inline double MaxComponent(const Vector3 &v)
    {
        return std::max(v.x, std::max(v.y, v.z));
    }

    inline int MaxDimension(const Vector3 &v)
    {
        return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
    }

    inline Vector3 Min(const Vector3 &p1, const Vector3 &p2)
    {
        return Vector3(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
                       std::min(p1.z, p2.z));
    }

    inline Vector3 Max(const Vector3 &p1, const Vector3 &p2)
    {
        return Vector3(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
                       std::max(p1.z, p2.z));
    }

    inline Vector3 Permute(const Vector3 &v, int x, int y, int z)
    {
        return Vector3(v[x], v[y], v[z]);
    }

    inline void CoordinateSystem(const Vector3 &v1, Vector3 *v2,
                                 Vector3 *v3)
    {
        if (std::abs(v1.x) > std::abs(v1.y))
            *v2 = Vector3(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
        else
            *v2 = Vector3(0, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
        *v3 = Cross(v1, *v2);
    }
}
