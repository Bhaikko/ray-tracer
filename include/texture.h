#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

// Abstract class for a texture
class texture
{
    public:
        virtual color value(double u, double v, const point3& p) const = 0;
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

        virtual color value(double u, double v, const point3& p) const override {
            return color_value;
        }
};

class checker_texture: public texture
{
    private:
        std::shared_ptr<texture> even;
        std::shared_ptr<texture> odd;

    public:
        checker_texture() {}

        checker_texture(std::shared_ptr<texture> _even, std::shared_ptr<texture> _odd)
            : even(_even), odd(_odd) {} 

        checker_texture(color c1, color c2)
            : even(std::make_shared<solid_color>(c1)), odd(std::make_shared<solid_color>(c2)) {} 

        virtual color value(double u, double v, const point3& p) const override;

};

#endif