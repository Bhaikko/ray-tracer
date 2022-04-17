#include "./../include/material.h"
#include "./../include/hittable.h"

bool lambertian::scatter(
    const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
) const {
    // Bouncing ray in random direction according to Diffuse surface
    
    // Can use the hemispherical unit vector independent of normal vector here
    // Current technique dependent on Normal vector
    
    // Diffuse material based on random ray dependent on normal
    vec3 scatter_direction = rec.normal + random_unit_vector();
    
    // Diffuse material based on random ray independent on normal
    // point3 target = rec.p + rec.normal + random_in_hemisphere(rec.normal);

    // Catch degenerate scatter direction 
    // if normal is directly opposite of scatter direction
    if (scatter_direction.near_zero()) {
        scatter_direction = rec.normal;
    }

    // Ray scattered from current hit point
    scattered = ray(rec.p, scatter_direction);

    attenuation = albedo;
    return true;
}

bool metal::scatter(
    const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
) const {
    // Reflecting ray based on Metal surface

    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);

    // Adding randomness to reflected ray based on fuzziness
    scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());

    attenuation = albedo;
    
    return (dot(scattered.direction(), rec.normal) > 0);
}

bool dielectric::scatter(
    const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
) const {
    attenuation = color(1.0, 1.0, 1.0);

    // Based on the whether the ray enters from less dense to more denser material or vice versa 
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    vec3 unit_direction = unit_vector(r_in.direction());
    vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

    scattered = ray(rec.p, refracted);
    return true;
}