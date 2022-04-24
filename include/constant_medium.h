#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "rtweekend.h"

#include "material.h"
#include "hittable.h"
#include "texture.h"

struct hit_record;

class constant_medium: public hittable 
{
    public:
        std::shared_ptr<hittable> boundary;
        std::shared_ptr<material> phase_function;
        double neg_inv_density;

    public: 
        constant_medium(std::shared_ptr<hittable> b, double d, std::shared_ptr<texture> a)
            : boundary(b), phase_function(std::make_shared<isotropic>(a)), neg_inv_density(-1 / d) {}

        constant_medium(std::shared_ptr<hittable> b, double d, color c)
            : boundary(b), phase_function(std::make_shared<isotropic>(c)), neg_inv_density(-1 / d) {}

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec
        ) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            return boundary->bounding_box(time0, time1, output_box);
        }
};

#endif