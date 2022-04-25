#include "./../include/rtweekend.h"
#include "./../include/rtw_stb_image.h"

#include "./../include/color.h"
#include "./../include/hittable_list.h"
#include "./../include/sphere.h"
#include "./../include/moving_sphere.h"
#include "./../include/camera.h"
#include "./../include/material.h"
#include "./../include/texture.h"
#include "./../include/aarect.h"
#include "./../include/box.h"
#include "./../include/constant_medium.h"
#include "./../include/bvh.h"

#include <iostream>

// Lerping ray color between Blue and White based on its y coordinate
color ray_color(const ray& r, const color& background, const hittable& world, int depth)
{
    hit_record rec;

    // Ray bounce limit, no light gathered
    if (depth <= 0) {
        // This causes the shadow when there are too many light bounces
        return color(0, 0, 0);
    }

    // If ray hits nothing, return black color
    // This is due to the fact that, only light sources will emit light
    if (!world.hit(r, 0.001, inifinity, rec)) {
        return background;
    }

    // Scattered is reflected ray upon hitting the object
    ray scattered;
    // Attentuation is how much enery of ray absorbed when hitting the surface
    color attenuation;
    // Emitted color based on object being light source
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        return emitted;
    }

    return  emitted + 
            attenuation * ray_color(scattered, background, world, depth - 1);
}

hittable_list random_scene()
{
    hittable_list world;

    std::shared_ptr<texture> checker = std::make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(checker)));

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

hittable_list two_spheres()
{
    hittable_list objects;

    std::shared_ptr<texture> checker = std::make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(std::make_shared<sphere>(point3(0, -10, 0), 10, std::make_shared<lambertian>(checker)));
    objects.add(std::make_shared<sphere>(point3(0, 10, 0), 10, std::make_shared<lambertian>(checker)));

    return objects;
}

hittable_list two_perlin_spheres()
{
    hittable_list objects;

    std::shared_ptr<noise_texture> pertext = std::make_shared<noise_texture>(4);
    objects.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, std::make_shared<lambertian>(pertext)));
    objects.add(std::make_shared<sphere>(point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

    return objects;
}

hittable_list earth()
{
    std::shared_ptr<image_texture> earth_texture = std::make_shared<image_texture>("./textures/earthmap.jpg");
    std::shared_ptr<lambertian> earth_surface = std::make_shared<lambertian>(earth_texture);
    std::shared_ptr<sphere> globe = std::make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);
    

    return hittable_list(globe);
}

hittable_list simple_light()
{
    hittable_list objects;

    std::shared_ptr<noise_texture> pertext = std::make_shared<noise_texture>(4);

    objects.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(pertext)));
    objects.add(std::make_shared<sphere>(point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

    std::shared_ptr<diffuse_light> difflight = std::make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    objects.add(std::make_shared<sphere>(point3(0, 7, 0), 2, difflight));

    return objects;
}

hittable_list cornell_box()
{
    hittable_list objects;

    std::shared_ptr<material> red =     std::make_shared<lambertian>(color(0.65, 0.05, 0.05));
    std::shared_ptr<material> white =   std::make_shared<lambertian>(color(0.73, 0.73, 0.73));
    std::shared_ptr<material> green =   std::make_shared<lambertian>(color(0.12, 0.45, 0.15));
    std::shared_ptr<material> light =   std::make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    std::shared_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, vec3(265,0,295));
    objects.add(box1);

    std::shared_ptr<hittable> box2 = std::make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, vec3(130,0,65));
    objects.add(box2);
    return objects;
}

hittable_list cornell_box_smoke()
{
    hittable_list objects;

    std::shared_ptr<material> red =     std::make_shared<lambertian>(color(0.65, 0.05, 0.05));
    std::shared_ptr<material> white =   std::make_shared<lambertian>(color(0.73, 0.73, 0.73));
    std::shared_ptr<material> green =   std::make_shared<lambertian>(color(0.12, 0.45, 0.15));
    std::shared_ptr<material> light =   std::make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<xz_rect>(113, 443, 127, 432, 554, light));

    std::shared_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, vec3(265,0,295));

    std::shared_ptr<hittable> box2 = std::make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, vec3(130,0,65));

    objects.add(std::make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    objects.add(std::make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));


    return objects;
}

hittable_list final_scene()
{
    hittable_list boxes1;
    std::shared_ptr<lambertian> ground = std::make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(std::make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list objects;
    objects.add(std::make_shared<bvh_node>(boxes1, 0, 1));

    std::shared_ptr<diffuse_light> light = std::make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(std::make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto moving_sphere_material = std::make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(std::make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(std::make_shared<sphere>(point3(260, 150, 45), 50, std::make_shared<dielectric>(1.5)));
    objects.add(std::make_shared<sphere>(
        point3(0, 150, 145), 50, std::make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = std::make_shared<sphere>(point3(360,150,145), 70, std::make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(std::make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<sphere>(point3(0, 0, 0), 5000, std::make_shared<dielectric>(1.5));
    objects.add(std::make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

    auto emat = std::make_shared<lambertian>(std::make_shared<image_texture>("./textures/earthmap.jpg"));
    objects.add(std::make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = std::make_shared<noise_texture>(0.1);
    objects.add(std::make_shared<sphere>(point3(220,280,300), 80, std::make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = std::make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<sphere>(point3::random(0,165), 10, white));
    }

    objects.add(std::make_shared<translate>(
        std::make_shared<rotate_y>(
            std::make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
            vec3(-100,270,395)
        )
    );

    return objects;
}

int main()
{
    // Image Configurations
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    // Ray tracing attributes
    int samples_per_pixel = 10;
    const int max_depth = 10;

    // World configurations
    hittable_list world;
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    double vfov = 40.0;
    double aperture = 0.0;
    color background(0, 0, 0);

    switch (0) {
        case 1:
            world = random_scene();
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            background = color(0.70, 0.80, 1.00);
            break;

        case 2:
            world = two_spheres();
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            background = color(0.70, 0.80, 1.00);
            break;

        case 3:
            world = two_perlin_spheres();
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            background = color(0.70, 0.80, 1.00);
            break;

        case 4:
            world = earth();
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            background = color(0.70, 0.80, 1.00);
            break;

        case 5:
            world = simple_light();
            samples_per_pixel = 100;
            background = color(0.0, 0.0, 0.0);
            lookfrom = point3(26, 3, 6);
            lookat = point3(0, 2, 0);
            vfov = 20.0;
            break;

        case 6:
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 400;
            // image_height = static_cast<int>(image_width / aspect_ratio);
            samples_per_pixel = 100;
            background = color(0, 0, 0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;

        case 7:
            world = cornell_box_smoke();
            aspect_ratio = 1.0;
            image_width = 400;
            samples_per_pixel = 100;
            background = color(0, 0, 0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;

        default:
        case 8:
            world = final_scene();
            aspect_ratio = 1.0;
            image_width = 500;
            image_height = static_cast<int>(image_width / aspect_ratio);
            samples_per_pixel = 500;
            background = color(0,0,0);
            lookfrom = point3(478, 278, -600);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;

    }

    // Camera Setup
    vec3 vup(0, 1, 0);
    // double dist_to_focus = (lookfrom - lookat).length();
    double dist_to_focus = 10.0;

    camera cam(
        lookfrom,
        lookat,
        vup,
        vfov,
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
                pixel_color += ray_color(r, background, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}