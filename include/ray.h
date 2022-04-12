#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray 
{
    public:
        point3 orig;        // Origin of Ray
        vec3 dir;           // Direction of Ray

    public:
        ray() {}

        ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction) {}

        point3 origin() const { return orig; }
        vec3 direction() const { return dir; }

        // According to Equation of Ray
        point3 at(double t) const
        {
            return orig + t * dir;
        }
};

#endif