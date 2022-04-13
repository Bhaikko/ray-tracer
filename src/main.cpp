#include <iostream>

#include "./../include/vec3.h"
#include "./../include/color.h"
#include "./../include/ray.h"

// Ray - Sphere Intersection
double hit_sphere(const point3& center, double radius, const ray& r)
{
    // Based on Ray-sphere intersection quadratic equation
    vec3 oc = r.origin() - center;

    // Optimisation for Root finding based on b = 2h
    double a = dot(r.direction(), r.direction());
    double half_b = dot(oc, r.direction());
    double c = oc.length_squared() - radius * radius;

    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

// Lerping ray color between Blue and White based on its y coordinate
color ray_color(const ray& r)
{
    // Checking Sphere intersection
    double t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        // Getting Normal vector from Point of intersection and center of Sphere
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }    

    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);    // Mapping from [-1, 1] -> [0, 1]

    // Blending between Blue and White
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    // Image Configurations
    const float aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    
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
            
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}