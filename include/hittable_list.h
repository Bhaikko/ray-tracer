#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

// List of hittable objects
class hittable_list: public hittable 
{
    public:
        std::vector<std::shared_ptr<hittable>> objects;

    public:
        hittable_list() {}
        hittable_list(std::shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

        // Returns true if ray hit any of these objects
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec
        ) const override;

        virtual bool bounding_box(
            double time0, double time1, aabb& output_box
        ) const override;
};

#endif