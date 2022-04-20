#include "./../include/moving_sphere.h"

bool moving_sphere::hit(
    const ray& r, 
    double t_min,
    double t_max,
    hit_record& rec 
) const {
    // Center at time t will be used for Ray intersection
    vec3 oc = r.origin() - center(r.time());
    double a = r.direction().length_squared();
    double half_b = dot(oc, r.direction());
    double c = oc.length_squared() - radius * radius;

    double discriminant = half_b * half_b - a * c; 
    if (discriminant < 0) {
        return false;
    }

    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in acceptable range
    // Based on Quadratic Root Formula
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;

        if (root < t_min || t_max < root) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    
    // Assigning Intersection Point Normal based on Ray direction
    vec3 outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);

    // Setting reference of current material to hit record of ray
    rec.mat_ptr = mat_ptr;

    return true;
}

point3 moving_sphere::center(double time) const 
{
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::bounding_box(
    double _time0, double _time1, aabb& output_box
) const {
    // Calculating bounding box at t0 and t1 seperately
    // Returning a bounding box for two prev calculated bounding box

    aabb box0(
        center(_time0) - vec3(radius, radius, radius),
        center(_time0) + vec3(radius, radius, radius)
    );

    aabb box1(
        center(_time1) - vec3(radius, radius, radius),
        center(_time1) + vec3(radius, radius, radius)
    );

    output_box = surrounding_box(box0, box1);
    return true;


}