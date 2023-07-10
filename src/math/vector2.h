#pragma once
#include <assert.h>
#include <cmath>
#include <iostream>
namespace raytracer
{
    class Vector2
    {
    public:
        double x, y;

        double operator[](int i) const
        {
            assert(i == 0 || i == 1);
            if (i == 0)
                return x;
            return y;
        }

        double &operator[](int i)
        {
             assert(i == 0 || i == 1);
            if (i == 0)
                return x;
            return y;
        }

        Vector2()
        {
            x = y = 0;
        }

        Vector2(double x, double y)
        {
            this->x = x;
            this->y = y;
        }

        Vector2(const Vector2 &v)
        {
            x = v.x;
            y = v.y;
        }

        Vector2 &operator=(const Vector2 &v)
        {
            x = v.x;
            y = v.y;
            return *this;
        }

        Vector2 operator+(const Vector2 &v) const
        {
            return Vector2(x + v.x, y + v.y);
        }
        Vector2 &operator+=(const Vector2 &v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }

        Vector2 operator-(const Vector2 &v) const
        {
            return Vector2(x - v.x, y - v.y);
        }

        Vector2 &operator-=(const Vector2 &v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        bool operator==(const Vector2 &v) const
        {
            return x == v.x && y == v.y;
        }

        bool operator!=(const Vector2 &v) const
        {
            return x != v.x || y != v.y;
        }

        Vector2 operator*(double s) const
        {
            return Vector2(s * x, s * y);
        }

        Vector2 &operator*=(double s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        Vector2 operator/(double f) const
        {
            assert(f != 0);
            double inv = 1 / f;
            return Vector2(x * inv, y * inv);
        }

        Vector2 &operator/=(double f)
        {
            assert(f != 0);
            double inv = 1 / f;
            x *= inv;
            y *= inv;
            return *this;
        }

        Vector2 operator-() const { return Vector2(-x, -y); }
        double LengthSquared() const { return x * x + y * y; }
        double Length() const { return std::sqrt(LengthSquared()); }
    };

    using Point2 = Vector2;

    inline std::ostream &operator<<(std::ostream &os, const Vector2 &v)
    {
        os << v.x << " " << v.y << " " << "\n";
        return os;
    }

    inline Vector2 operator*(double s, const Vector2 &v)
    {
        return v * s;
    }

    inline Vector2 Abs(const Vector2 &v)
    {
        return Vector2(std::abs(v.x), std::abs(v.y));
    }

    inline double Dot(const Vector2 &v1, const Vector2 &v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    inline double AbsDot(const Vector2 &v1, const Vector2 &v2)
    {
        return std::abs(Dot(v1, v2));
    }

    inline Vector2 Normalize(const Vector2 &v)
    {
        return v / v.Length();
    }

    inline double MinComponent(const Vector2 &v)
    {
        return std::min(v.x, v.y);
    }

    inline double MaxComponent(const Vector2 &v)
    {
        return std::max(v.x, v.y);
    }

    inline Vector2 Min(const Vector2 &p1, const Vector2 &p2)
    {
        return Vector2(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
    }

    inline Vector2 Max(const Vector2 &p1, const Vector2 &p2)
    {
        return Vector2(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
    }
}
