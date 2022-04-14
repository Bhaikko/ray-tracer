#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>

// Constants
const double inifinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degress_to_radians(double degress)
{
    return degress * pi / 180.0;
}

// Common Headers
#include "ray.h"
#include "vec3.h"

#endif
