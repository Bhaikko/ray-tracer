#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream& out, color pixel_color)
{
    // out used to write to stream object
    // cout, err, etc can be passed

    // Transforming [0, 1] to [0, 255]
    out << static_cast<int>(255.999 * pixel_color.x()) << ' ' 
        << static_cast<int>(255.999 * pixel_color.y()) << ' ' 
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif