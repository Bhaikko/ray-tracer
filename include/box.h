#ifndef BOX_H
#define BOX_H

#include "rtweekend.h"

#include "aarect.h"
#include "hittable_list.h"

// Box Build using 6 Axis aligned planes
class box: public hittable 
{
    public:
        point3 box_min;
        point3 box_max;
        hittable_list sides;

    public:
        box() {}
        box(const point3& p0, const point3& p1, std::shared_ptr<material> ptr);

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif