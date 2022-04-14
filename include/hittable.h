#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// Defining an abstract class for anything a ray might hit

// Ray hit attributes
struct hit_record {
    point3 p;           // Point of intersection
    vec3 normal;        // Normal at point of intersection
    double t;           // Parametric parameter at which the intersection occured for the ray
    bool front_face;    // Tracking the intersection is on front of face or back

    inline void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        // Determining based on dot product
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable 
{
    public:
        // t_min and t_max for intersection in valid interval
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif