#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "material.h"

// Sphere class
class sphere: public hittable
{
    public:
        point3 center;
        double radius;
        std::shared_ptr<material> mat_ptr;  // Material assigned to sphere

    public:
        sphere() {}
        sphere(point3 cen, double r, std::shared_ptr<material> m): center(cen), radius(r), mat_ptr(m) {};

        // Contains Ray Intersection method for this object
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec
        ) const override;

        // Generating Bounding box for sphere
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif