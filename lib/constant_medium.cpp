#include "./../include/constant_medium.h"

// To Print debugging messages for Rays
// #define CONSTANT_MEDIUM_DEBUG

bool constant_medium::hit(const ray& r, double t_min, double t_max, hit_record& rec) const 
{
    // Print occasional samples while debugging
#ifdef CONSTANT_MEDIUM_DEBUG
    const bool debugging = random_double() < 0.00001;
#endif

    hit_record rec1, rec2;
    if (!boundary->hit(r, -inifinity, inifinity, rec1)) {
        return false;
    }

    if (!boundary->hit(r, rec1.t + 0.0001, inifinity, rec2)) {
        return false;
    }

#ifdef CONSTANT_MEDIUM_DEBUG
    if (debugging) {
        std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << "\n";
    }
#endif

    // Handling Boundary Conditions
    if (rec1.t < t_min) {
        rec1.t = t_min;
    }

    if (rec2.t > t_max) {
        rec2.t = t_max;
    }

    if (rec1.t >= rec2.t) {
        return false;
    }

    if (rec1.t < 0) {
        rec1.t = 0;
    }

    const double ray_length = r.direction().length();
    const double distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const double hit_distance = neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary) {
        return false;
    }

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

#ifdef CONSTANT_MEDIUM_DEBUG
    if (debugging) {
        std::cerr   << "hit_distance = " << hit_distance << "\n" 
                    << "rec.t = " << rec.t << "\n"
                    << "rec.p = " << rec.p << "\n";
    }
#endif

    rec.normal = vec3(1, 0, 0);     // Arbitary Normal
    rec.front_face = true;          // Arbitary 
    rec.mat_ptr = phase_function;

    return true;

}