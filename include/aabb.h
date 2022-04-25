#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb 
{
    public:
        // Since AABB are standard axis aligned
        // Therefor, minimum and maximum point req and plane can be constructed 
        // As planes will always will be parallel to Standard Axes
        point3 minimum;
        point3 maximum;

    public:
        aabb() {} 
        aabb(const point3& a, const point3& b);

        point3 min() const { return minimum; }
        point3 max() const { return maximum; }

        bool hit(const ray& r, double t_min, double t_max) const;
};

inline aabb surrounding_box(aabb box0, aabb box1) 
{
    point3 small(
        fmin(box0.min().x(), box1.min().x()),
        fmin(box0.min().y(), box1.min().y()),
        fmin(box0.min().z(), box1.min().z())
    );

    point3 big(
        fmax(box0.max().x(), box1.max().x()),
        fmax(box0.max().y(), box1.max().y()),
        fmax(box0.max().z(), box1.max().z())
    );

    return aabb(small, big);
}

#endif