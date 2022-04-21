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
    scattered = ray(rec.p, scatter_direction, r_in.time());

    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}

bool metal::scatter(
    const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
) const {
    // Reflecting ray based on Metal surface

    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);

    // Adding randomness to reflected ray based on fuzziness
    scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());

    attenuation = albedo;
    
    return (dot(scattered.direction(), rec.normal) > 0);
}

bool dielectric::scatter(
    const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
) const {
    // Glass surface absorbs nothing
    attenuation = color(1.0, 1.0, 1.0);

    // Based on the whether the ray enters from less dense to more denser material or vice versa 
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    vec3 unit_direction = unit_vector(r_in.direction());

    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
        // Total Internal Refraction
        direction = reflect(unit_direction, rec.normal);
    } else {
        // Normal Refraction based on Snell's law
        direction = refract(unit_direction, rec.normal, refraction_ratio);
    }

    scattered = ray(rec.p, direction, r_in.time());
    return true;
}

double dielectric::reflectance(double cosine, double ref_idx)
{
    // Using Schlick's approximation for reflectance
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;

    return r0 + (1 - r0) * pow((1 - cosine), 5);    
}