#pragma once
#include <assert.h>
#include <cmath>
#include <iostream>

namespace raytracer
{
    class Color
    {
    public:
        float r, g, b;

        bool is_black()
        {
            return fabs(r) < 0.001 && fabs(g) < 0.001 && fabs(b) < 0.001;
        }

        float operator[](int i) const
        {
            assert(i >= 0 && i <= 2);
            if (i == 0)
                return r;
            if (i == 1)
                return g;
            return b;
        }

        float &operator[](int i)
        {
            assert(i >= 0 && i <= 2);
            if (i == 0)
                return r;
            if (i == 1)
                return g;
            return b;
        }

        Color()
        {
            r = g = b;
        }

        Color(float r, float g, float b)
        {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        Color(const Color &c)
        {
            r = c.r;
            g = c.g;
            b = c.b;
        }

        Color &operator=(const Color &c)
        {
            r = c.r;
            g = c.g;
            b = c.b;
            return *this;
        }

        Color operator+(const Color &c) const
        {
            return Color(r + c.r, g + c.g, b + c.b);
        }

        Color operator+=(const Color &c)
        {
            r += c.r;
            g += c.g;
            b += c.b;
            return *this;
        }

        Color operator-(const Color &c)
        {
            return Color(r - c.r, g - c.g, b - c.b);
        }

        Color operator-=(const Color &c)
        {
            r -= c.r;
            g -= c.g;
            b -= c.b;
            return *this;
        }

        bool operator==(const Color &c) const
        {
            return r == c.r && g == c.g && b == c.b;
        }

        bool operator!=(const Color &c) const
        {
            return r != c.r && g != c.g && b != c.b;
        }

        Color operator*(const Color &c) const
        {
            return Color(r * c.r, g * c.g, b * c.b);
        }

        Color operator*=(float s)
        {
            r *= s;
            g *= s;
            b *= s;
            return *this;
        }

        Color operator*=(const Color &c)
        {
            r *= c.r;
            g *= c.g;
            b *= c.b;
            return *this;
        }

        Color operator/(float s) const
        {
            return Color(r / s, g / s, b / s);
        }

        Color operator/=(const Color &c)
        {
            r /= c.r;
            g /= c.g;
            b /= c.b;
            return *this;
        }

        static const Color red, green, dark_green, blue, white, black, yellow, cyan,
            brown, grey, purple, orange;

        // http://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
        Color gamma_corrected()
        {
            Color c;
            c.r = std::max(1.055 * pow(r, 0.416666667) - 0.055, 0.0);
            c.g = std::max(1.055 * pow(g, 0.416666667) - 0.055, 0.0);
            c.b = std::max(1.055 * pow(b, 0.416666667) - 0.055, 0.0);
            return c;
        }
    };

    inline std::ostream &operator<<(std::ostream &out, const Color &c)
    {
        out << c.r << " " << c.g << " " << c.b << "\n";
        return out;
    }

    inline Color operator*(float s, const Color &c)
    {
        return Color(s * c.r, s * c.g, s * c.b);
    }

    inline Color operator*(const Color &c, float s)
    {
        return Color(s * c.r, s * c.g, s * c.b);
    }

    inline Color Clamp(Color c, float minVal, float maxVal)
    {
        c.r = std::min(std::max(c.r, minVal), maxVal);
        c.g = std::min(std::max(c.g, minVal), maxVal);
        c.b = std::min(std::max(c.b, minVal), maxVal);
        return c;
    }
}