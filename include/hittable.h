#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// Defining an abstract class for anything a ray might hit

// Ray hit attributes
struct hit_record {
    point3 p;           // Point of intersection
    vec3 normal;        // Normal at point of intersection
    double t;           // Parametric parameter at which the intersection occured for the ray
};

class hittable 
{
    public:
        // t_min and t_max for intersection in valid interval
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif