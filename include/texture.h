#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

// Abstract class for a texture
class texture
{
    public:
        virtual color(double u, double v, const point& p) const = 0;
};

class solid_color: public texture 
{
    private:
        color color_value;

    public:
        solid_color() {}
        solid_color(color c) : color_value(c) {}

        solid_color(double red, double green, double blue)
            : solid_color(color(red, green, blue)) {}

        virtual color_value(double u, double v, const point3& p) const override {
            return color_value;
        }
};

#endif