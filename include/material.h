#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

// Abstract class for a material that 
// 1. Produces a scattered ray
// 2. If scattered, how much the ray should be attenuated
// Material will tell us how rays interact with the surface
class material
{
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

#endif