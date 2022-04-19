#include "./../include/rtweekend.h"

#include "./../include/color.h"
#include "./../include/hittable_list.h"
#include "./../include/sphere.h"
#include "./../include/moving_sphere.h"
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

hittable_list random_scene()
{
    hittable_list world;

    std::shared_ptr<lambertian> ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));

    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            // Randomising Material and Sphere positions
            double choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // Diffuse

                    // Spawning Movable spheres that transition between two posision
                    vec3 albedo = color::random() * color::random();
                    sphere_material = std::make_shared<lambertian>(albedo);

                    vec3 center2 = center + vec3(0, random_double(0, 0.5), 0);

                    world.add(std::make_shared<moving_sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material
                    ));
                } else if (choose_mat < 0.95) {
                    // Metal
                    vec3 albedo = color::random(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // Glass

                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    std::shared_ptr<dielectric> material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    std::shared_ptr<lambertian> material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    std::shared_ptr<metal> material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
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
    hittable_list world = random_scene();

    // std::shared_ptr<lambertian> material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // std::shared_ptr<lambertian> material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // std::shared_ptr<dielectric> material_left   = std::make_shared<dielectric>(1.5);
    // std::shared_ptr<metal> material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    // world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    // world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    // world.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
    
    // Camera Setup
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    // double dist_to_focus = (lookfrom - lookat).length();
    double dist_to_focus = 10.0;
    double aperture = 0.1;

    camera cam(
        lookfrom,
        lookat,
        vup,
        20,
        aspect_ratio,
        aperture,
        dist_to_focus,
        0.0,
        1.0
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