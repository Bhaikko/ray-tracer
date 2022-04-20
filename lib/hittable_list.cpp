#include "./../include/hittable_list.h"

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    // Checking if the ray hits any of the objects in the list
    hit_record temp_rec;

    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const std::shared_ptr<hittable>& object: objects) {
        // Hitting while considering the closest point on Ray
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(
    double time0, double time1, aabb& output_box
) const {
    if (objects.empty()) {
        return false;
    }

    aabb temp_box;
    bool first_box = true;

    for (const std::shared_ptr<hittable>& object: objects) {
        if (!object->bounding_box(time0, time1, temp_box)) {
            return false;
        }

        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}