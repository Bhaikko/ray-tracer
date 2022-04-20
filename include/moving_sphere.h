#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"
#include "hittable.h"

class moving_sphere: public hittable 
{
    public:
        point3 center0, center1;
        double time0, time1;
        double radius;
        std::shared_ptr<material> mat_ptr;

    public:
        moving_sphere() {} ;

        moving_sphere(
            point3 cen0, point3 cen1, double _time0, double _time1,
            double r, std::shared_ptr<material> m 
        ) : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
        {} ;

    public:
        virtual bool hit (
            const ray& r, double t_min, double t_max, hit_record& rec
        ) const override;

        virtual bool bounding_box(
            double _time0, double _time1, aabb& output_box
        ) const override;

        // Returning center at time t based on linear interpolation
        point3 center(double time) const;
};

#endif