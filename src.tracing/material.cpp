//
//  material.cpp
//  src.common
//
//  Created by 杨丰 on 2020/11/3.
//

#include "material.h"
#include "hittable.h"

bool lambertian::scatter(const ray& r_in, const hit_record& rec,
                         scatter_record& srec) const {
    srec.is_specular = false;
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
    return true;
}

double lambertian::scattering_pdf(const ray& r_in, const hit_record& rec,
                                  const ray& scattered) const {
    auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
    return cosine < 0 ? 0 : cosine/pi;
}

bool metal::scatter(const ray& r_in, const hit_record& rec,
                    scatter_record& srec) const {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    srec.specular_ray =
    ray(rec.p, reflected + fuzz*random_in_unit_sphere(), r_in.time());
    srec.attenuation = albedo;
    srec.is_specular = true;
    srec.pdf_ptr = nullptr;
    return true;
}

bool dielectric::scatter(const ray& r_in, const hit_record& rec,
                         scatter_record& srec) const {
    srec.is_specular = true;
    srec.pdf_ptr = nullptr;
    srec.attenuation = color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
    
    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
    
    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 direction;
    
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, refraction_ratio);
    
    srec.specular_ray = ray(rec.p, direction, r_in.time());
    return true;
}

//MARK:- Light
color diffuse_light::emitted(const ray& r_in, const hit_record& rec,
                             double u, double v, const point3& p) const {
    if (!rec.front_face)
        return color(0,0,0);
    return emit->value(u, v, p);
}
