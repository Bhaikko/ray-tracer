#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb 
{
    public:
        point3 minimum;
        point3 maximum;

    public:
        aabb() {} 
        aabb(const point3& a, const point3& b);

        point3 min() const { return minimum; }
        point3 max() const { return maximum; }

        bool hit(const ray& r, double t_min, double t_max) const;
};

#endif