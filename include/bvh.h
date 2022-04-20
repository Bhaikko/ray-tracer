#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"

// This class itself will be responsible for both
// BVH tree and BVH node with Root node as entry to tree
class bvh_node: public hittable
{
    public:
        // Children can be anytype of hittables
        // bvh_nodes, sphere or any other hittable
        std::shared_ptr<hittable> left;
        std::shared_ptr<hittable> right;

        aabb box;

    public:
        bvh_node() {};

        bvh_node(const hittable_list& list, double time0, double time1)
            : bvh_node(list.objects, 0, list.objects.size(), time0, time1) {}

        bvh_node(
            const std::vector<std::shared_ptr<hittable>>& src_objects,
            size_t start,
            size_t end,
            double time0,
            double time1 
        );

        // Indiviual Node Hit check
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

#endif