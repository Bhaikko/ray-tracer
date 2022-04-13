#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

// Sphere class
class sphere: public hittable
{
    public:
        point3 center;
        double radius;

    public:
        sphere() {}
        sphere(point3 cen, double r): center(cen), radius(r) {};

        // Contains Ray Intersection method for this object
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec
        ) const override;
};

#endif