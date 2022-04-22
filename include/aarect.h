#ifndef AARECT_H
#define AARECT_H

// Defining Rectangle planes

#include "rtweekend.h"
#include "hittable.h"

class xy_rect: public hittable 
{
    public:
        // k is constant for z coordinate
        double x0, x1, y0, y1, k;

        std::shared_ptr<material> mp;

    public:
        xy_rect() {}

        xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif