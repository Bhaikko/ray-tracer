#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "material.h"
#include "aabb.h"

// Defining an abstract class for anything a ray might hit

// Ray hit attributes
struct hit_record {
    point3 p;           // Point of intersection
    vec3 normal;        // Normal at point of intersection
    std::shared_ptr<material> mat_ptr;  // Reference to the material the ray hits
    double t;           // Parametric parameter at which the intersection occured for the ray
    bool front_face;    // Tracking the intersection is on front of face or back
    double u;           // UV coordinates for texture lookup
    double v;


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

        // Generating Bounding Box for a Hittable
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

class translate: public hittable 
{
    public:
        std::shared_ptr<hittable> ptr;
        vec3 offset;

    public:
        translate(std::shared_ptr<hittable> p, const vec3& displacement)
            : ptr(p), offset(displacement) {}

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec
        ) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

class rotate_y: public hittable 
{
    public:
        std::shared_ptr<hittable> ptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        aabb bbox;

    public:
        rotate_y(std::shared_ptr<hittable> p, double angle);

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif