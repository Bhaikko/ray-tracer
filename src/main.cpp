#include "./../include/rtweekend.h"

#include "./../include/color.h"
#include "./../include/hittable_list.h"
#include "./../include/sphere.h"
#include "./../include/camera.h"
#include "./../include/material.h"

#include <iostream>

// Lerping ray color between Blue and White based on its y coordinate
color ray_color(const ray& r, const hittable& world, int depth)
{
    hit_record rec;

    // Ray bounce limit, no light gathered
    if (depth <= 0) {
        // This causes the shadow when there are too many light bounces
        return color(0, 0, 0);
    }

    if (world.hit(r, 0.001, inifinity, rec)) {
        ray scattered;

        // Attentuation is how much energy of ray absorbed when hitting the surface
        color attentuation;

        if (rec.mat_ptr->scatter(r, rec, attentuation, scattered)) {
            return attentuation * ray_color(scattered, world, depth - 1);
        }

        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());

    // Mapping from [-1, 1] -> [0, 1]
    double t = 0.5 * (unit_direction.y() + 1.0);   

    // Blending between Blue and White
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    // Image Configurations
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Ray tracing attributes
    const int samples_per_pixel = 10;
    const int max_depth = 10;

    // World configurations
    double R = cos(pi / 4);
    hittable_list world;

    std::shared_ptr<lambertian> material_left = std::make_shared<lambertian>(color(0, 0, 1));
    std::shared_ptr<lambertian> material_right = std::make_shared<lambertian>(color(1, 0, 0));

    world.add(std::make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(std::make_shared<sphere>(point3( R, 0, -1), R, material_right));

    // Camera Setup
    camera cam(
        point3(-2, 2, 1),
        point3(0, 0, -1),
        vec3(0, 1, 0),
        90,
        aspect_ratio
    );

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Render
    for (int j = image_height - 1; j >= 0; --j) {
        // Using std::cerr since std::cout stream is output to image file
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);

            // Anti-aliasing
            for (int s = 0; s < samples_per_pixel; ++s) {
                // Sending Ray from origin towards Viewport based on Offsets 
                double u = double(i + random_double()) / (image_width - 1);
                double v = double(j + random_double()) / (image_height - 1);

                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}