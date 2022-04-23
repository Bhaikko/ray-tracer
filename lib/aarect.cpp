#include "./../include/aarect.h"

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    double t = (k - r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max) {
        return false;
    }

    double x = r.origin().x() + t * r.direction().x();
    double y = r.origin().y() + t * r.direction().y();

    if (x < x0 || x > x1 || y < y0 || y > y1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;

    vec3 outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    double t = (k - r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max) {
        return false;
    }

    double x = r.origin().x() + t * r.direction().x();
    double z = r.origin().z() + t * r.direction().z();

    if (x < x0 || x > x1 || z < z0 || z > z1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;

    vec3 outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    double t = (k - r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max) {
        return false;
    }

    double y = r.origin().y() + t * r.direction().y();
    double z = r.origin().z() + t * r.direction().z();

    if (y < y0 || y > y1 || z < z0 || z > z1) {
        return false;
    }

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;

    vec3 outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

bool xy_rect::bounding_box(double time0, double time1, aabb& output_box) const
{
    // Padding attached to bounding box of rectangle to add width to it
    output_box = aabb(
        point3(x0, y0, k - 0.0001),
        point3(x1, y1, k + 0.0001)
    );

    return true;
}

bool xz_rect::bounding_box(double time0, double time1, aabb& output_box) const
{
    // Padding attached to bounding box of rectangle to add width to it
    output_box = aabb(
        point3(x0, k - 0.0001, z0),
        point3(x1, k + 0.0001, z1)
    );

    return true;
}

bool yz_rect::bounding_box(double time0, double time1, aabb& output_box) const
{
    // Padding attached to bounding box of rectangle to add width to it
    output_box = aabb(
        point3(k - 0.0001, y0, z0),
        point3(k + 0.0001, y1, z1)
    );

    return true;
}

