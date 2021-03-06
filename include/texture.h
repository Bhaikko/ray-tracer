#ifndef TEXTURE_H
#define TEXTURE_H

#include "./../helper/stb_image.h"

#include "rtweekend.h"
#include "perlin.h"

#include <iostream>

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

class noise_texture: public texture
{
    public:
        perlin noise;
        // Scale added to adjust frequency
        double scale;

    public:
        noise_texture() {}
        noise_texture(double sc): scale(sc) {}

        virtual color value(double u, double v, const point3& p) const override {
            return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
        }
};

class image_texture: public texture 
{
    private:
        unsigned char* data;
        int width, height;
        int bytes_per_scanline;

    public:
        const static int bytes_per_pixel = 3;

        image_texture()
            : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

        image_texture(const char* filename);

        ~image_texture() { 
            delete data;
        }

        virtual color value(double u, double v, const vec3& p) const override;

};

#endif