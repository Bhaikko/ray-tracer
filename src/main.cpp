#include "./../include/rtweekend.h"

#include "./../include/color.h"
#include "./../include/hittable_list.h"
#include "./../include/sphere.h"

#include <iostream>

// Lerping ray color between Blue and White based on its y coordinate
color ray_color(const ray& r, const hittable& world)
{
    hit_record rec;
    if (world.hit(r, 0, inifinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
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
    const float aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World 
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));
    
    // Camera Setup
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1.0f;

    // Viewport Setup
    vec3 origin = point3(0, 0, 0);
    vec3 horizontal = vec3(viewport_width, 0, 0);
    vec3 vertical = vec3(0, viewport_height, 0);

    // Viewport center will be at (0, 0) in x-y plane and in -z direction
    vec3 lower_left_corner =    origin - 
                                horizontal / 2 - 
                                vertical / 2 - 
                                vec3(0, 0, focal_length);



    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Render
    for (int j = image_height - 1; j >= 0; --j) {
        // Using std::cerr since std::cout stream is output to image file
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            // Sending Ray from origin towards Viewport based on Offsets 
            double u = double(i) / (image_width - 1);
            double v = double(j) / (image_height - 1);

            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            
            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}