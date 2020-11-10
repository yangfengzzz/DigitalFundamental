//
//  material.h
//  DigitalRender
//
//  Created by 杨丰 on 2020/11/3.
//

#ifndef material_h
#define material_h

#include "pbr.h"
#include "pdf.h"
#include "texture.h"

struct hit_record;

struct scatter_record {
    ray specular_ray;
    bool is_specular;
    color attenuation;
    shared_ptr<pdf> pdf_ptr;
};

class material {
public:
    virtual color emitted(const ray& r_in, const hit_record& rec,
                          double u, double v, const point3& p) const {
        return color(0,0,0);
    }
    
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         scatter_record& srec) const {
        return false;
    }
    
    virtual double scattering_pdf(const ray& r_in, const hit_record& rec,
                                  const ray& scattered) const {
        return 0;
    }
};

class lambertian : public material {
public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}
    
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         scatter_record& srec) const override;
    
    double scattering_pdf(const ray& r_in, const hit_record& rec,
                          const ray& scattered) const override;
    
public:
    shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         scatter_record& srec) const override;
    
public:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}
    
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         scatter_record& srec) const override;
    
public:
    double ir; // Index of Refraction
    
private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

//MARK:- Lights
class diffuse_light : public material  {
public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}
    
    virtual color emitted(const ray& r_in, const hit_record& rec,
                          double u, double v, const point3& p) const override;
    
public:
    shared_ptr<texture> emit;
};

//MARK:- Volume
class isotropic : public material {
public:
    isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
    isotropic(shared_ptr<texture> a) : albedo(a) {}
    
#if 0
    // Issue #669
    // This method doesn't match the signature in the base `material` class, so this one's
    // never actually called. Disabling this definition until we sort this out.
    
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         color& attenuation, ray& scattered) const override {
        scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
#endif
    
public:
    shared_ptr<texture> albedo;
};

#endif /* material_h */
