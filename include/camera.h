#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera
{
    public:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;

        // Time interval to send rays in between, Shutter open and close time
        double time0, time1;

    public:
        /**
         * @param vfov - Vertical field of view in degress
         */
        camera(
            point3 lookfrom, 
            point3 lookat, 
            vec3 vup, 
            double vfov, 
            double aspect_ratio,
            double aperture,
            double focus_dist,
            double _time0 = 0,
            double _time1 = 0
        );

        ray get_ray(double s, double t) const;
};

#endif