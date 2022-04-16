#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel)
{
    // out used to write to stream object
    // cout, err, etc can be passed

    // Instead of dividing color to map to [0, 1] each time
    // Dividing by number of samples at end after accumulating the sample color
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // Gamma Correction with Scaling
    double scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Transforming [0, 1] to [0, 255]
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' ' 
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' ' 
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif