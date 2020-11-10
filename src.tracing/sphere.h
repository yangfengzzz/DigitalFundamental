//
//  sphere.h
//  DigitalRender
//
//  Created by 杨丰 on 2020/11/2.
//

#ifndef sphere_h
#define sphere_h

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> m)
    : center(cen), radius(r), mat_ptr(m) {};
    
    virtual bool hit(const ray& r, double t_min,
                     double t_max, hit_record& rec) const override;
    
    virtual bool bounding_box(double time0, double time1,
                              aabb& output_box) const override;
    
    virtual double pdf_value(const point3& o, const vec3& v) const override;
    virtual vec3 random(const point3& o) const override;
    
public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
    
private:
    static void get_sphere_uv(const point3& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
        
        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;
        
        u = phi / (2*pi);
        v = theta / pi;
    }
};


#endif /* sphere_h */
