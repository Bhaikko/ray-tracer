#include "./../include/camera.h"

camera::camera(
    point3 lookfrom, 
    point3 lookat, 
    vec3 vup, 
    double vfov, 
    double aspect_ratio,
    double aperture,
    double focus_dist,
    double _time0,
    double _time1
) {
    // Viewport Setup
    double theta = degress_to_radians(vfov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left_corner = origin - 
                        horizontal / 2 - 
                        vertical / 2 -
                        focus_dist * w;

    lens_radius = aperture / 2;
    time0 = _time0;
    time1 = _time1;
}

ray camera::get_ray(double s, double t) const
{
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();

    return ray(
        origin + offset,
        lower_left_corner + s * horizontal + t * vertical - origin - offset,
        // This specifies the time at which the ray will sample for the intersection of moving sphere
        random_double(time0, time1)     
    );
}