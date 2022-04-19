#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray 
{
    public:
        point3 orig;        // Origin of Ray
        vec3 dir;           // Direction of Ray
        double tm;          // Time at which Ray exists

    public:
        ray() {}

        ray(const point3& origin, const vec3& direction, double time = 0.0)
            : orig(origin), dir(direction), tm(time) {}

        point3 origin() const { return orig; }
        vec3 direction() const { return dir; }
        double time() const { return tm; }

        // According to Equation of Ray
        point3 at(double t) const
        {
            return orig + t * dir;
        }
};

#endif