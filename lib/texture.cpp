#include "./../include/texture.h"

color checker_texture::value(double u, double v, const point3& p) const 
{
    // Product of Sines will give a checkered pattern
    double sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if (sines < 0) {
        return odd->value(u, v, p);
    } else {
        return even->value(u, v, p);
    }
}

// Reading Texture file context 
image_texture::image_texture(const char* filename)
{
    int components_per_pixel = bytes_per_pixel;

    data = stbi_load(
        filename, &height, &width, &components_per_pixel, components_per_pixel
    );

    if (!data) {
        std::cerr << "ERROR: could not load texture image file '" << filename << "'\n";
        width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

color image_texture::value(double u, double v, const vec3& p) const 
{
    // If no texture data found, return Cyan as Debugging aid
    if (data == nullptr) {
        return color(0, 1, 1);
    }

    // Clamp input texture coordinates to [0, 1] x [1, 0]
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0);   // Flip V to image coordinates

    int i = static_cast<int>(u * width);
    int j = static_cast<int>(v * height);

    // Clamping integer mapping
    if (i >= width) {
        i = width - 1;
    }

    if (j >= height) {
        j = height - 1;
    }

    const double color_scale = 1.0 / 255.0;
    unsigned char* pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

    return color(
        color_scale * pixel[0],
        color_scale * pixel[1],
        color_scale * pixel[2]
    );
}
